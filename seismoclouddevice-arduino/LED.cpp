
#include <Arduino.h>
#include "common.h"

uint8_t LED::greenLedPin;
uint8_t LED::yellowLedPin;
uint8_t LED::redLedPin;
uint8_t LED::blinkingLed = 0;
unsigned long LED::lastBlink;
bool LED::lastBlinkStatus;

void LED::init(uint8_t greenLedPin, uint8_t yellowLedPin, uint8_t redLedPin) {
  LED::greenLedPin = greenLedPin;
  LED::yellowLedPin = yellowLedPin;
  LED::redLedPin = redLedPin;

  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
}

void LED::green(bool isOn) {
   digitalWrite(greenLedPin, (isOn ? HIGH : LOW));
}

void LED::red(bool isOn) {
  digitalWrite(redLedPin, (isOn ? HIGH : LOW));
}

void LED::yellow(bool isOn) {
  digitalWrite(yellowLedPin, (isOn ? HIGH : LOW));
}

void LED::setLedBlinking(uint8_t pin) {
  lastBlinkStatus = LOW;
  lastBlink = 0;
  if(pin == greenLedPin || pin == redLedPin || pin == yellowLedPin) {
    blinkingLed = pin;
  }
}

void LED::clearLedBlinking() {
  digitalWrite(blinkingLed, LOW);
  blinkingLed = 0;
}

void LED::startupBlink() {
  for(int i=0; i < 10; i++) {
    LED::green(true);
    LED::red(true);
    LED::yellow(true);
    delay(50);
    LED::green(false);
    LED::red(false);
    LED::yellow(false);
    delay(50);
  }
}

void LED::tick() {
  if(blinkingLed != 0 && millis() - lastBlink >= 500) {
    lastBlinkStatus = (lastBlinkStatus == HIGH ? LOW : HIGH);
    digitalWrite(blinkingLed, lastBlinkStatus);
    lastBlink = millis();
  }
}

void LED::dispose() { }

