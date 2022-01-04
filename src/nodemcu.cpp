#include <WiFiManager.h>
#include "nodemcu.h"

void NodeMCU_init() {
  memset(buffer, 0, BUFFER_SIZE);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);

#ifdef DEBUG
  Debug("MAC Address: ");
  printMACAddress();
  Debug("Hostname: ");
  Debugln(hostname);
#endif

  Debug("Creating config portal in ");
  Debugln(CFGSSID);

  WiFi.setSleep(false);

  // Set the hostname directly in the Wi-Fi chip to bypass WiFiManager (see note below)
  WiFi.hostname(hostname);

  WiFiManager wifi;

  /* WiFiManager will support this in the next stable release
  wifi.setHostname(hostname);
  */

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
}
