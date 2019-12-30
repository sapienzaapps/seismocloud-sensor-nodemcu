
#include "common.h"

unsigned long lastAliveMs = 0;

void setup() {
#ifdef DEBUG
  // start serial port:
  Serial.begin(115200);
#endif

  LED_init();
  LED_startupBlink();

  Debug(F("SeismoCloud-Arduino version "));
  Debugln(VERSION);

  Debugln(F("Init seismometer and calibrate"));
  seismometerInit();

  NodeMCU::begin();

  checkForUpdates();

  if (!apiConnect()) {
    soft_restart();
  }

  Debugln(F("Updating Time"));

  apiTimeReq();
  for(int i=0; i < 200 && getUNIXTime() == 0; i++) {
    apiTick();
    delay(100);
  }
  if (getUNIXTime() == 0) {
    Debugln(F("Timeout updating time, reboot"));
    delay(2000);
    soft_restart();
    while(true);
  }

  Debug(F("Local time: "));
#ifdef DEBUG
  printUNIXTime();
#endif
  Debugln();

  Debugln(F("Init cmd interface"));
  commandInterfaceInit();

  Debugln(F("Send keep-alive"));
  apiAlive();
  lastAliveMs = millis();

  Debugln(F("Boot completed"));
  Debugln();
  LED_startupBlink();
  LED_green(true);
}

void loop() {
  commandInterfaceTick();

  apiTick();

  // Calling alive every 14 minutes
  if((millis() - lastAliveMs) >= 840000) {
#ifdef DEBUG
    Debug(F("Keepalive at "));
    printUNIXTime();
    Debugln();
#endif

    // Trigger API alive
    apiAlive();
    lastAliveMs = millis();

	  // Trigger NTP update
    apiTimeReq();
  }

  // Detection
  seismometerTick();
}
