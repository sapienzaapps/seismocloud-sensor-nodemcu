
#include <ESP8266HTTPClient.h>
#include <WebSocketsClient.h>
#include <WiFiClientSecure.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "MPU6050.h"
#include "debug_print.h"
#include "nodemcu.h"

#ifdef DEBUG
const String seismoCloudServer("http://192.168.183.234:8000");
#else

const String seismoCloudServer();

#endif

WiFiClient client;
WebSocketsClient webSocket;

String rpiHostName;
uint16_t rpiPort = 80;

void extractValue(char* dst, int maxlength, const char *src, char separator) {
  char *nextsep = strchr(src, separator);
  if (nextsep == NULL) {
    strncpy(dst, src, min(maxlength, (int)strlen(src)+1));
  } else {
    int valueLength = nextsep - src;
    strncpy(dst, src, min(maxlength, valueLength+1));
  }
}

void parseConfig(const char *payload) {
  const uint16_t MAX_BUF_SIZE = 300;
  char buf[MAX_BUF_SIZE+1] = {0};

  while (payload != NULL) {
    if (strncmp("rpiaddress=", payload, 11) == 0) {
      extractValue(buf, MAX_BUF_SIZE, payload+11, '\n');
      rpiHostName = String(buf);
    } else if (strncmp("rpiport=", payload, 8) == 0) {
      extractValue(buf, MAX_BUF_SIZE, payload+8, '\n');
      int arg = atoi(buf);
      if (arg > 0 && arg < 65536) {
        rpiPort = arg;
      }
    }

    payload = strchr(payload, '\n');
    if (payload != NULL) {
      payload++;
    }
  }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_ERROR:
      Debugln("[API] WS internal error");
      break;
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
      Debugln((char *) payload);
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
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT_TEXT_START:
      Debugln(F("[API] Fragments not supported"));
      break;
  }
}

bool apiConnect() {
  HTTPClient http;

  Debugln(F("[API] Getting config"));
  if (http.begin(client, seismoCloudServer + "/probes/v1/" + String(deviceid) + "/config")) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      // OK
      String payload = http.getString();
      Debug(F("[API] Configuration retrieved: "));
      Debugln(payload);

      // TODO: parse config
      parseConfig(payload.c_str());
      return true;
    } else if (httpCode <= 0) {
      Debug(F("[API] Connection error: "));
      Debugln(http.errorToString(httpCode).c_str());
      return false;
    } else {
      // Unexpected response from server
      Debug(F("[API] Unexpected status code from server: "));
      Debugln(httpCode);
      return false;
    }

    http.end();
  } else {
    Debugln(F("[API] HTTP library error: can't initialize HTTP client"));
    return false;
  }

  Debugln(F("[API] Connecting to web socket"));
  webSocket.begin(rpiHostName, rpiPort, "/probes/v1/" + String(deviceid) + "/stream");
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
  webSocket.sendBIN((uint8_t *) buf, sizeof(double) * 3);
  // anche Tmp?
}

void apiTick() {
  webSocket.loop();
}

void apiDisconnect() {
  webSocket.disconnect();
}
