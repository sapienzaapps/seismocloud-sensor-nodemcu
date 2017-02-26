
#include "nodemcu.h"
#ifdef IS_ESP

void NodeMCU::begin() {
#ifdef DEBUG
    Serial.print("Connecting to WiFi ");
    Serial.println("SeismoCloud");
#endif

  WiFiManager wifi;
  wifi.autoConnect("SeismoCloud");

#ifdef DEBUG
  char currentIp[17];
  // if you get here you have connected to the WiFi
  Serial.println("Wifi connected!");
  Serial.print("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf(currentIp, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Serial.println(currentIp);
  
  // Getting MAC Address
  uint8_t macAddress[6];
  WiFi.macAddress(macAddress);
  setMACAddress(macAddress);
  Serial.print("MAC Address: ");
  Serial.println(getDeviceId());
#endif
}

#endif
