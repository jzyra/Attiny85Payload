#include "DigiKeyboardFr.h"

#define MAIL "mail@example.com"
#define OBJECT "Object"
#define HELLO "Hello,"
#define BYE "Bye,"
#define MESSAGE "Message"

#define TAB 0x2B

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
  DigiKeyboardFr.println("outlook /c ipm.note /m "MAIL);
  DigiKeyboardFr.delay(time_run);
  DigiKeyboardFr.print(OBJECT);
  DigiKeyboardFr.sendKeyStroke(TAB);
  #ifdef HELLO
  DigiKeyboardFr.print(HELLO);
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER);
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER);
  #endif
  DigiKeyboardFr.print(MESSAGE);
  #ifdef BYE
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER);
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER);
  DigiKeyboardFr.print(BYE);
  #endif
  DigiKeyboardFr.sendKeyStroke(KEY_ENTER, MOD_CONTROL_LEFT);
  digitalWrite(led_id, HIGH);
  for (;;) {
  }
}
