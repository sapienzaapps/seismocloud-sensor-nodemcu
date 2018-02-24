
#include "common.h"

void LED_init() {
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
}

void LED_green(bool isOn) {
   digitalWrite(LED_GREEN, (isOn ? HIGH : LOW));
}

void LED_red(bool isOn) {
  digitalWrite(LED_RED, (isOn ? HIGH : LOW));
}

void LED_yellow(bool isOn) {
  digitalWrite(LED_YELLOW, (isOn ? HIGH : LOW));
}

void LED_startupBlink() {
  for(int i=0; i < 10; i++) {
    LED_green(true);
    LED_red(true);
    LED_yellow(true);
    delay(50);
    LED_green(false);
    LED_red(false);
    LED_yellow(false);
    delay(50);
  }
  LED_green(false);
  LED_red(false);
  LED_yellow(false);
}


