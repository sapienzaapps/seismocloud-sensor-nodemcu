
#include "common.h"
#include "external-ip.h"

unsigned long debug_lastms = 0;

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
  Debugln(F("Init seismometer and calibrate"));
  LED_accel_calibr();
  seismometerInit();

  // Initialize NodeMCU WiFi
  LED_wait_net_cfg();
  NodeMCU_init();

  // Check for firmware updates
  LED_update();
  checkForUpdates();

  // Get public IP
  getExternalIP();

  LED_connection();
  // Connect to API server
  if (!apiConnect()) {
    LED_lost_api();
    soft_restart();
  }

  // Force time update using MQTT
  Debugln(F("Force update local time (20s timeout)"));
  apiTimeReq();
  for(int i=0; i < 200 && getUNIXTime() == 0; i++) {
    apiTick();
    delay(100);
  }
  if (getUNIXTime() == 0) {
    Debugln(F("Timeout updating time, reboot"));
    soft_restart();
  }

  // Starting local discovery interface
  Debugln(F("Init cmd interface"));
  commandInterfaceInit();

  // Sending first keep alive to server
  Debugln(F("Send keep-alive"));
  apiAlive();

  Debugln(F("Boot completed"));
  Debugln();
  LED_startup_blink();
  LED_ready();

  // Issue a new NTP sync as esp8266 is not well stable
  apiTimeReq();
}

void loop() {
  // Execute local discovery events
  commandInterfaceTick();

  // Execute API events
  apiTick();

  // Execute seismometer events
  seismometerTick();

#ifdef DEBUG
  if (millis() - debug_lastms > 1000) {
    Debug(F("Current time: "));
    printUNIXTime();
    Debugln();

    debug_lastms = millis();
  }
#endif
}
