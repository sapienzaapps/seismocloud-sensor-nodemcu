
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
  memset(buffer, 0, BUFFER_SIZE);
  // if you get here you have connected to the WiFi
  Serial.println("Wifi connected!");
  Serial.print("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf((char*)buffer, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Serial.println((char*)buffer);
  
  memset(buffer, 0, 16);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);
  Serial.print("MAC Address: ");
  printMACAddress();
#endif
}

#endif
