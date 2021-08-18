
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "nodemcu.h"
#include "debug_print.h"

byte ethernetMac[6] = { 0 };
char deviceid[13] = { 0 };

void NodeMCU_init() {
  Debugln("Creating config portal Wi-Fi network SSID: SeismoCloud");

  WiFiManager wifi;
  wifi.setConfigPortalTimeout(180);

  bool connected = wifi.autoConnect("SeismoCloud");
  if (!connected) {
    NodeMCU_reboot();
  }

#ifdef DEBUG
  char buffer[20] = { 0 };
  // if you get here you have connected to the WiFi
  Debugln("Wifi connected!");
  Debug("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf(buffer, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Debugln(buffer);
#endif

  // Getting MAC Address and use it as device ID
  WiFi.macAddress(ethernetMac);
  snprintf(deviceid, 13, "%02x%02x%02x%02x%02x%02x", ethernetMac[0], ethernetMac[1], ethernetMac[2], ethernetMac[3], ethernetMac[4], ethernetMac[5]);

#ifdef DEBUG
  Debug("MAC Address: ");
  snprintf(buffer, 18, "%02x:%02x:%02x:%02x:%02x:%02x", ethernetMac[0], ethernetMac[1], ethernetMac[2], ethernetMac[3], ethernetMac[4], ethernetMac[5]);
  Debugln(buffer);
#endif
}

void NodeMCU_reboot() {
  ESP.restart();
  while(true);
}
