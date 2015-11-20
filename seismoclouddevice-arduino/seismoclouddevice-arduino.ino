
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "common.h"

#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5

unsigned long lastAliveMs = 0;
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;

void setup() {
  LED::init(LED_GREEN, LED_YELLOW, LED_RED);
  LED::green(true);
  LED::red(true);
  LED::yellow(true);
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println(F("Booting SeismoCloudDevice-Arduino sketch"));

  // Check config, load MAC and lat/lon
  loadConfig();

  byte ethernetMac[6];
  getMACAddress(ethernetMac);

  if(isZero(ethernetMac, 6)) {
    Serial.print(F("No MAC Address configured - generating a new one: "));
    generateMACAddress(ethernetMac);
    Serial.println(macToString(ethernetMac).c_str());
  } else {
    Serial.print(F("Configured MAC Address: "));
    Serial.println(macToString(ethernetMac).c_str());
  }

  // give the ethernet module time to boot up:
  delay(1000);

  Serial.println(F("Enabling Ethernet link..."));
  Ethernet.begin(ethernetMac);
  
  // print the Ethernet board/shield's IP address:
  Serial.print(F("My IP address: "));
  Serial.println(Ethernet.localIP());

  Serial.println(F("Updating NTP Time"));
  do {
    updateNTP();
    setBootTime(getUNIXTime());
    if(getBootTime() == 0) {
      Serial.println(F("NTP update failed, retrying in 5 seconds..."));
      delay(5 * 1000);
    }
  } while(getBootTime() == 0);

  Serial.print(F("Local time is "));
  printUNIXTime();
  Serial.println();

  Serial.println(F("Init command interface"));
  commandInterfaceInit();

  Serial.println(F("Send first keep-alive to server..."));
  httpAliveRequest();
  lastAliveMs = millis();
  Serial.println(F("Keep-alive sent"));

  if(getLatitude() == 0 && getLongitude() == 0) {
    LED::green(false);
    LED::red(false);
    LED::yellow(false);
    LED::setLedBlinking(LED_YELLOW);
    Serial.println(F("No position available, waiting for GPS from phone App"));
    do {
      commandInterfaceTick();
      LED::tick();
    } while(getLatitude() == 0 && getLongitude() == 0);
    Serial.print(F("New position: "));
    LED::clearLedBlinking();
    LED::green(true);
    LED::red(true);
    LED::yellow(true);
  }
  
  Serial.print(F("GPS Latitude: "));
  Serial.print(getLatitudeAsString());
  Serial.print(F(" - Longitude: "));
  Serial.println(getLongitudeAsString());

  Serial.println(F("Init seismometer and calibrate"));
  seismometerInit();

  Serial.print(F("Boot completed at "));
  printUNIXTime();
  Serial.println();
  LED::startupBlink();
  LED::green(true);
}

void loop() {
  // Update NTP (if necessary)
  updateNTP();

  commandInterfaceTick();
  LED::tick();

  // Calling alive every 14 minutes
  if((millis() - lastAliveMs) >= 840000) {
    Serial.print(F("Keepalive sent at "));
    printUNIXTime();
    Serial.println();
    
    httpAliveRequest();
    lastAliveMs = millis();
    
    Serial.print(F("Keepalive ACK at "));
    printUNIXTime();
    Serial.println();
  }

  // Detection
  seismometerTick();
  if(millis() - lastProbeMs >= 1000) {
    lastProbeMs = millis();
    setProbeSpeedStatistic(probeCount);
    probeCount = 0;
  }
  probeCount++;
}

