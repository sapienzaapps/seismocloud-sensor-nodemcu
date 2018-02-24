
#include "common.h"

unsigned long lastAliveMs = 0;

#ifdef DEBUG
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;
#endif

void setup() {
#ifdef DEBUG
  // start serial port:
  Serial.begin(115200);
#endif

  LED_init();
  LED_startupBlink();

  Debug(F("SeismoCloud-Arduino version "));
  Debugln(VERSION);

  checkEEPROM();

  // Switching SPI to ethernet
  selectEthernet();

  Debugln(F("Init seismometer and calibrate"));
  seismometerInit();

  Debugln(F("Loading config"));
  // Check config, load MAC and lat/lon
  loadConfig();

#ifdef IS_ARDUINO
  checkMACAddress();

  Debug(F("MAC Address: "));
#ifdef DEBUG
  printMACAddress();
#endif

  // give the ethernet module time to boot up:
  delay(1000);

  Debugln(F("Enabling Ethernet"));
  Ethernet.begin(ethernetMac);
  
  // Check Ethernet link
  if(Ethernet.localIP() == INADDR_NONE) {
    Debugln(F("Ethernet failed to load"));
    delay(2000);
    soft_restart();
    while(true);
  } else {
    Debug(F("My IP address: "));
    Debugln(Ethernet.localIP());
  }
#endif
#ifdef IS_ESP
  NodeMCU::begin();
#endif
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
    Debug(F("Keepalive at "));
#ifdef DEBUG
    printUNIXTime();
#endif
    Debugln();

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

