
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include "common.h"

unsigned long lastAliveMs = 0;
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;

void setup() {
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
  Serial.println(getBootTime());

  Serial.println(F("Init command interface"));
  commandInterfaceInit();

  Serial.println(F("Send first keep-alive to server..."));
  httpAliveRequest();
  lastAliveMs = millis();
  Serial.println(F("Keep-alive sent"));

  if(getLatitude() == 0 && getLongitude() == 0) {
    Serial.println(F("No position available, waiting for GPS from phone App"));
    do {
      commandInterfaceTick();
    } while(getLatitude() == 0 && getLongitude() == 0);
    Serial.print(F("New position: "));
  }
  
  Serial.print(F("GPS Latitude: "));
  Serial.print(getLatitude());
  Serial.print(F(" - Longitude: "));
  Serial.println(getLongitude());

  Serial.println(F("Init seismometer and calibrate"));
  seismometerInit();

  Serial.println(F("Send keep-alive to server..."));
  httpAliveRequest();
  lastAliveMs = millis();
  Serial.println(F("Keep-alive sent, boot completed"));
}

void loop() {
  // Update NTP (if necessary)
  updateNTP();

  commandInterfaceTick();

  // Calling alive every 14 minutes
  if(millis() - lastAliveMs >= 1000 * 60 * 14) {
    Serial.println(F("Keepalive"));
    httpAliveRequest();
    lastAliveMs = millis();
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

