
#include "common.h"

// Initialize device
void setup() {
#ifdef DEBUG
  delay(1000);
  // start serial port for debug
  Serial.begin(115200);
#endif

  Wire.begin(WIRE_SDA, WIRE_SCL);
  //Wire.setClock(400000L);

  displayInit();

  LED_init();

  Debug(F("SeismoCloud-NodeMCU version "));
  Debugln(VERSION);

  oled.print(F("SeismoCloud "));
  oled.println(VERSION);

  // Power up and calibrate the seismometer
  Debugln(F("Init seismometer and calibrate"));
  oled.print(F("Calibration..."));
  LED_accel_calibr();
  seismometerInit();
  oled.println(F("ok"));

  // Initialize NodeMCU WiFi
  oled.print(F("Wi-Fi setup..."));
  LED_wait_net_cfg();
  NodeMCU_init();
  oled.println(F("ok"));

  // Check for firmware updates
  oled.print(F("Check for updates..."));
  LED_update();
  checkForUpdates();
  oled.println(F("ok"));

  // Connect to API server
  oled.print(F("Connecting..."));
  LED_connection();
  if (!apiConnect()) {
    LED_lost_api();
    soft_restart();
  }
  oled.println(F("ok"));

  // Force time update using MQTT
  Debugln(F("Force update local time (20s timeout)"));
  oled.print(F("Updating time..."));
  apiTimeReq();
  for(int i=0; i < 200 && getUNIXTime() == 0; i++) {
    apiTick();
    delay(100);
  }
  if (getUNIXTime() == 0) {
    Debugln(F("Timeout updating time, reboot"));
    soft_restart();
  }
  oled.println(F("ok"));

  // Starting local discovery interface
  Debugln(F("Init cmd interface"));
  commandInterfaceInit();

  // Sending first keep alive to server
  Debugln(F("Send keep-alive"));
  apiAlive();

  Debugln(F("Boot completed"));
  Debugln();
  oled.println(F("Boot completed"));
  LED_startup_blink();
  LED_ready();
  oled.clear();
}

void loop() {
  // Execute local discovery events
  commandInterfaceTick();

  // Execute API events
  apiTick();

  // Execute seismometer events
  seismometerTick();
}
