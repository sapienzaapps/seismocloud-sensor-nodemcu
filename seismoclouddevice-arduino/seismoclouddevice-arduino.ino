
#include "common.h"

unsigned long lastAliveMs = 0;

#ifdef DEBUG
unsigned long lastProbeMs = 0;
uint32_t probeCount = 0;
#endif

void setup() {
  // start serial port:
  Serial.begin(115200);

  LED_init();
  LED_startupBlink();

#ifdef DEBUG
  Serial.print(F("SeismoCloud-Arduino version "));
  Serial.println(VERSION);
#endif

  checkEEPROM();

#ifdef DEBUG
  Serial.println(F("Init seismometer and calibrate"));
#endif
  seismometerInit();

#ifdef DEBUG
  Serial.println(F("Loading config"));
#endif
  // Check config, load MAC and lat/lon
  loadConfig();

#ifdef IS_ARDUINO
  checkMACAddress();

#ifdef DEBUG
  Serial.print(F("MAC Address: "));
#endif
  printMACAddress();

  // give the ethernet module time to boot up:
  delay(1000);

#ifdef DEBUG
  Serial.println(F("Enabling Ethernet"));
#endif
  Ethernet.begin(buffer);
  
  // Check Ethernet link
  if(Ethernet.localIP() == INADDR_NONE) {
#ifdef DEBUG
    Serial.println(F("Ethernet failed to load"));
#endif
    delay(2000);
    soft_restart();
    while(true);
  } else {
#ifdef DEBUG
    Serial.print(F("My IP address: "));
    Serial.println(Ethernet.localIP());
#endif
  }
#endif
#ifdef IS_ESP
  NodeMCU::begin();
#endif
  apiConnect();

#ifdef DEBUG
  Serial.println(F("Updating Time"));
#endif

  apiTimeReq();
  for(int i=0; i < 200 && getUNIXTime() == 0; i++) {
    apiTick();
    delay(100);
  }
  if (getUNIXTime() == 0) {
#ifdef DEBUG
    Serial.println(F("Timeout updating time, reboot"));
#endif
    delay(2000);
    soft_restart();
    while(true);
  }

#ifdef DEBUG
  Serial.print(F("Local time: "));
  printUNIXTime();
  Serial.println();
#endif

#ifdef DEBUG
  Serial.println(F("Init cmd interface"));
#endif
  commandInterfaceInit();

#ifdef DEBUG
  Serial.println(F("Send keep-alive"));
#endif
  apiAlive();
  lastAliveMs = millis();

#ifdef DEBUG
  Serial.println(F("Boot completed"));
  Serial.println();
#endif
  LED_startupBlink();
  LED_green(true);
}

void loop() {
  commandInterfaceTick();

  apiTick();

  // Calling alive every 14 minutes
  if((millis() - lastAliveMs) >= 840000) {
#ifdef DEBUG
    Serial.print(F("Keepalive at "));
    printUNIXTime();
    Serial.println();
#endif

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

