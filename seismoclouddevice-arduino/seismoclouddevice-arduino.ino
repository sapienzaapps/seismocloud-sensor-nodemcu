
#include <SPI.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "common.h"

#ifdef IS_ARDUINO
#include <Ethernet.h>
#endif
#ifdef IS_ESP
#include <Wire.h>
//ESP8266 Core WiFi Library
#include <ESP8266WiFi.h>
//Local DNS Server used for redirecting all requests to the configuration portal
#include <DNSServer.h>
//Local WebServer used to serve the configuration portal
#include <ESP8266WebServer.h>
//https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <WiFiManager.h>
#endif

unsigned long lastAliveMs = 0;

#ifdef DEBUG
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;
#endif

// TODO: https://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html

void setup() {
  // start serial port:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  LED::init(LED_GREEN, LED_YELLOW, LED_RED);
  LED::startupBlink();

  Serial.print(F("SeismoCloud-Arduino version "));
  Serial.println(VERSION);

  checkEEPROM();

  Serial.println(F("Init seismometer and calibrate"));
  seismometerInit();

  Serial.println(F("Loading config"));
  // Check config, load MAC and lat/lon
  loadConfig();

#ifdef IS_ARDUINO
  byte ethernetMac[6];
  getMACAddress(ethernetMac);

  Serial.print(F("MAC Address: "));
  Serial.println(getDeviceId());

  // give the ethernet module time to boot up:
  delay(1000);

  Serial.println(F("Enabling Ethernet"));
  Ethernet.begin(ethernetMac);
  
  // Check Ethernet link
  if(Ethernet.localIP() == INADDR_NONE) {
    Serial.println(F("Ethernet failed to load"));
    delay(2000);
    soft_restart();
    while(true);
  } else {
    Serial.print(F("My IP address: "));
    Serial.println(Ethernet.localIP());
  }
#endif
#ifdef IS_ESP
  NodeMCU::begin();
#endif
  apiConnect();

  Serial.println(F("Updating Time"));

  apiTimeReq();
  for(int i=0; i < 200 && getUNIXTime() == 0; i++) {
    apiTick();
    delay(100);
  }
  if (getUNIXTime() == 0) {
    Serial.println(F("Timeout updating time, reboot"));
    delay(2000);
    soft_restart();
    while(true);
  }
  
  setBootTime(getUNIXTime());

  Serial.print(F("Local time: "));
  printUNIXTime();
  Serial.println();

  Serial.println(F("Init cmd interface"));
  commandInterfaceInit();

  Serial.println(F("Send keep-alive"));
  apiAlive();
  lastAliveMs = millis();

  Serial.println(F("Boot completed"));
  Serial.println();
  LED::startupBlink();
  LED::green(true);
}

void loop() {
  commandInterfaceTick();
  LED::tick();

  apiTick();

  // Calling alive every 14 minutes
  if((millis() - lastAliveMs) >= 840000) {
    Serial.print(F("Keepalive at "));
    printUNIXTime();
    Serial.println();
    
    apiAlive();
    lastAliveMs = millis();
  }

  // Detection
  seismometerTick();
#ifdef DEBUG
  if(millis() - lastProbeMs >= 1000) {
    lastProbeMs = millis();
    setProbeSpeedStatistic(probeCount);
    probeCount = 0;
  }
  probeCount++;
#endif
}

