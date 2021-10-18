#include "external-ip.h"
#include "common.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

char externalIP[16] = { 0 };

void getExternalIP() {
  WiFiClient client;
  HTTPClient http;
  if (http.begin(client, "http://api.ipify.org/")) {
    if (http.GET() == HTTP_CODE_OK) {
      snprintf(externalIP, 16, "%s", http.getString().c_str());

      Debug("Remote IP: ");
      Debugln(externalIP);
    }
    http.end();
    client.stop();
  } else {
    Debugln("Remote IP: connection failed");
  }
}
