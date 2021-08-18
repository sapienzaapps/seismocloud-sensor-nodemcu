#ifndef UNIT_TEST

#include <Arduino.h>
#include "nodemcu.h"
#include "LED.h"
#include "MPU6050.h"
#include "api.h"
#include "debug_print.h"

unsigned long lastAcceleroTick = 0;
uint16_t probeSpeedHz = 100;

// Initialize device
void setup() {
#ifdef DEBUG
  delay(1000);
  // start serial port for debug
  Serial.begin(115200);
#endif

  LED_init();

  Debug(F("SeismoCloud-Arduino version "));
  Debugln(VERSION);

  // Power up and calibrate the seismometer
  Debugln(F("Init accelerometer and calibrate"));
  LED_accel_calibr();
  if (!MPU6050_begin()) {
    Debugln(F("Accelerometer init error"));
    NodeMCU_reboot();
  }

  // Initialize NodeMCU WiFi
  LED_wait_net_cfg();
  NodeMCU_init();

  LED_connection();
  // Connect to API server
  if (!apiConnect()) {
    LED_lost_api();
    NodeMCU_reboot();
  }

  Debugln(F("Boot completed"));
  Debugln();
  LED_startup_blink();
  LED_ready();
}

void loop() {
  // Execute API events
  apiTick();

  // Execute seismometer events
  if (probeSpeedHz > 0 && millis()-lastAcceleroTick < (1000/probeSpeedHz)) {
    return;
  }
  lastAcceleroTick = millis();

  MPU6050_probe();
  apiStream();
}

#endif
