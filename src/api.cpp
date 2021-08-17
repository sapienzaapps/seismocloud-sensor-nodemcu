
#include "api.h"
#include <WiFiClientSecure.h>

WiFiClient client;
WebSocketsClient webSocket;

uint16_t port = 3000;
String ipAddress = "192.168.183.234";
String deviceId = ":deviceid";

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Debugln("[API] Disconnected");
      break;
    case WStype_CONNECTED:
      Debugln("[API] Connected");
      // send message to server when Connected
      webSocket.sendTXT("Connected");
      break;
    case WStype_TEXT:
      Debug("[API] Rcvd text: ");
      Debugln((char*)payload);
      break;
    case WStype_BIN:
      Debug("[API] Rcvd binary len: ");
      Debugln(length);
      break;
    case WStype_PING:
      Debugln("[API] Ping");
      break;
    case WStype_PONG:
      Debugln("[API] Pong");
      break;
  }
}

boolean apiConnect() {
  HTTPClient http;
  Debugln(F("[API] Getting config"));
  if (http.begin(client, "http://" + ipAddress + ":" + port + "/probes/v1/" + deviceId + "/config")) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      // OK
      String payload = http.getString();
      Debug(F("Configuration retrieved: "));
      Debugln(payload);

      // TODO: parse config
    } else if (httpCode <= 0) {
      Debug(F("HTTP library error: "));
      Debugln(http.errorToString(httpCode).c_str());
    } else {
      // Unexpected response from server
    }

    http.end();
  } else {
    // TODO: error initializing HTTP
  }

  Debugln(F("[API] Connecting to web socket"));
  webSocket.begin(ipAddress, port, "/probes/v1/" + deviceId + "/stream");
  webSocket.onEvent(webSocketEvent);
  //webSocket.setAuthorization("user", "Password");
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(10000, 3000, 2);
  webSocket.setExtraHeaders();

  // TODO: timeout
  while (!webSocket.isConnected()) {
    delay(50);
    webSocket.loop();
  }

  return webSocket.isConnected();
}

void apiStream() {
  double buf[3];
  buf[0] = acceleroX;
  buf[1] = acceleroY;
  buf[2] = acceleroZ;
  webSocket.sendBIN((uint8_t*)buf, sizeof(double)*3);
  // acceleroX, acceleroY, acceleroZ
  // anche Tmp?
}

void apiTick() {
  webSocket.loop();
}

void apiDisconnect() {
  webSocket.disconnect();
}
