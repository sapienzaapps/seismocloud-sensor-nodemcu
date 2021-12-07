#include <WiFiManager.h>
#include "nodemcu.h"

void NodeMCU_init() {
  Debug("Creating config portal in ");
  Debugln(CFGSSID);

  WiFi.setSleep(false);

  WiFiManager wifi;
  wifi.setConfigPortalTimeout(180);

  bool connected = wifi.autoConnect(CFGSSID);
  if (!connected) {
    ESP.restart();
  }

#ifdef DEBUG
  // if you get here you have connected to the WiFi
  Debugln("Wifi connected!");
  Debug("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf((char*)buffer, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Debugln((char*)buffer);
#endif

  memset(buffer, 0, BUFFER_SIZE);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);

#ifdef DEBUG
  Debug("MAC Address: ");
  printMACAddress();
#endif
}
