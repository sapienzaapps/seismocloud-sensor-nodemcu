
#include "nodemcu.h"
#ifdef IS_ESP

void NodeMCU::begin() {
  Debug("Connecting to WiFi ");
  Debugln(CFGSSID);

  WiFiManager wifi;
  wifi.setConfigPortalTimeout(180);
  bool connected = wifi.autoConnect(CFGSSID);
  if (!connected) {
    ESP.restart();
  }

#ifdef DEBUG
  memset(buffer, 0, BUFFER_SIZE);
  // if you get here you have connected to the WiFi
  Debugln("Wifi connected!");
  Debug("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf((char*)buffer, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Debugln((char*)buffer);
#endif

  memset(buffer, 0, 16);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);
  Debug("MAC Address: ");
#ifdef DEBUG
  printMACAddress();
#endif
}

#endif
