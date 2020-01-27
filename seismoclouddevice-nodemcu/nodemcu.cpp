
#include "nodemcu.h"

void NodeMCU_apcallback(WiFiManager *wifi) {
	oled.clear();
	oled.println(F("Please configure me!"));
	oled.println();
	oled.println(F("Wi-Fi: SeismoCloud"));
	oled.println(F("No password!"));
	oled.println(F("http://192.168.4.1/"));
	LED_wificonfig_blink();
	LED_wait_net_cfg();
}

void NodeMCU_init() {
  Debug("Creating config portal in ");
  Debugln(CFGSSID);

  WiFiManager wifi;
  wifi.setConfigPortalTimeout(180);
  wifi.setAPCallback(&NodeMCU_apcallback);

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

  memset(buffer, 0, BUFFER_SIZE);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);

#ifdef DEBUG
  Debug("MAC Address: ");
  printMACAddress();
#endif
}
