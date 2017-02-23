
/**
 * You can find user-configurable switch into  common.h
 * Puoi trovare alcune configurazioni di compilazione in  common.h
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "common.h"

unsigned long lastAliveMs = 0;

#ifdef DEBUG
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;
#endif

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  LED::init(LED_GREEN, LED_YELLOW, LED_RED);
  LED::startupBlink();
  LED::green(false);
  LED::red(false);
  LED::yellow(false);

  Serial.print(F("SeismoCloud-Arduino version "));
  Serial.println(VERSION);

  checkEEPROM();

  Serial.println(F("Init seismometer and calibrate"));
  seismometerInit();

  Serial.println(F("Loading config"));
  // Check config, load MAC and lat/lon
  loadConfig();

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

