
#include "api.h"

#ifdef IS_ARDUINO
EthernetClient ethernetClient;
#endif
#ifdef IS_ESP
WiFiClient ethernetClient;
#endif
PubSubClient mqttClient(ethernetClient);

unsigned long lastNTPTime = 0;
unsigned long lastNTPMillis = 0;

void apiCallback(char* topic, byte* payload, unsigned int len) {
  if (len == 0) {
    return;
  }

  switch (payload[0]) {
    case API_TIMERESP:
      memcpy(&lastNTPTime, payload + 1, 4);
      Debug(F("Time:"));
      Debugln(lastNTPTime);
      lastNTPMillis = millis();
      break;
    case API_CFG:
      if (len < 7) {
        break;
      }
      /**
         Payload (after type):
         Offset       Byte      Desc
         1            4         Sigma (IEEE 754)
         5            1         Host len
         6            len(host) Hostname
         6+len(host)  1         Path len
         7+len(host)  len(path) Script path
      */
#ifndef DONT_UPDATE
      byte hlen, plen;
      memcpy(&hlen, payload + 5, 1);
      if (hlen > 0) {
        memcpy(&plen, payload + 6 + hlen, 1);
        if (plen > 0) {
          *(payload + 6 + hlen) = 0;
          memset(buffer, 0, BUFFER_SIZE);
          memcpy(buffer, payload + 6 + hlen + 1, plen);
          if (update((char*)(payload + 6), (char*)buffer)) {
            Debugln(F("Update succeded, reboot"));
            soft_restart();
          } else {
            Debugln(F("Update failed"));
          }
        }
      }
#endif

      float sigma;
      memcpy(&sigma, payload + 1, 4);
      setSigmaIter(sigma);
      Debug(F("Setting sigma to "));
      Debugln(sigma);
      resetLastPeriod();
      break;
    case API_REBOOT:
      apiDisconnect();
      soft_restart();
      break;
    default:
      break;
  }
}

unsigned long getUNIXTime() {
  if (lastNTPTime == 0) {
    return 0;
  }
  unsigned long diffms = millis() - lastNTPMillis;
  return lastNTPTime + (diffms / 1000);
}

void apiDisconnect() {
  // Will message for disconnection
  memset(buffer, 0, BUFFER_SIZE);
  byte j = 0;
  buffer[j] = API_DISCONNECT;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  // END Will message
  mqttClient.publish("server", buffer, j, false);
  mqttClient.disconnect();
}

boolean apiConnect() {
  // Will message for disconnection
  memset(buffer, 0, BUFFER_SIZE);
  byte j = 0;
  buffer[j] = API_DISCONNECT;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  // END Will message

  mqttClient.setServer("192.0.2.60", 1883);
  mqttClient.setCallback(apiCallback);
  mqttClient.connect((char*)(buffer + 2), "test1", "test1", "server", 0, 0, (char*)buffer);

#ifdef DEBUG
  switch (mqttClient.state()) {
    case MQTT_CONNECTION_TIMEOUT:
      Debugln(F("[MQTT] Timeout"));
      break;
    case MQTT_CONNECTION_LOST:
      Debugln(F("[MQTT] Connection lost"));
      break;
    case MQTT_CONNECT_FAILED:
      Debugln(F("[MQTT] Connection failed"));
      break;
    case MQTT_DISCONNECTED:
      Debugln(F("[MQTT] Disconnected"));
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      Debugln(F("[MQTT] Bad protocol"));
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      Debugln(F("[MQTT] Bad client ID"));
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      Debugln(F("[MQTT] Server unavailable"));
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      Debugln(F("[MQTT] Bad credentials"));
      break;
    case MQTT_CONNECT_UNAUTHORIZED:
      Debugln(F("[MQTT] Unauthorized"));
      break;
    case MQTT_CONNECTED:
      Debugln(F("[MQTT] Connected"));
      break;
  }
#endif

  if (mqttClient.state() == MQTT_CONNECTED) {
    memset(buffer, 0, BUFFER_SIZE);
    snprintf((char*)buffer, BUFFER_SIZE, "device-%02x%02x%02x%02x%02x%02x", ethernetMac[0], ethernetMac[1], ethernetMac[2], ethernetMac[3], ethernetMac[4], ethernetMac[5]);
    Debug("Subscribing ");
    Debugln((char*)buffer);
    mqttClient.subscribe((char*)buffer, 1);
    return true;
  } else {
    return false;
  }
}

void apiTick() {
  if (!mqttClient.loop()) {
    delay(3000);
    apiConnect();
  }
}

void apiAlive() {
  /**
     Keep alive message format:
     Position   Size      Value
     0          1         1 (type=alive)
     1          1         device id length
     2          len(did)  device id as ASCII string
     #          1         device model length
     #          len(mod)  device model
     #          1         version length
     #          len(ver)  version as ASCII string

  */
  memset(buffer, 0, BUFFER_SIZE);
  byte j = 0;
  buffer[j] = API_KEEPALIVE;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  // Model
  buffer[j] = strlen(MODEL);
  j++;
  memcpy(buffer + j, MODEL, strlen(MODEL));
  j += strlen(MODEL);

  // Version
  buffer[j] = strlen(VERSION);
  j++;
  memcpy(buffer + j, VERSION, strlen(VERSION));
  j += strlen(VERSION);

  mqttClient.publish("server", buffer, j, false);

#ifdef IS_ESP
  memset(buffer, 0, BUFFER_SIZE);
  j = 0;
  buffer[j] = API_TEMPERATURE;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  // Temperature
  buffer[j] = sizeof(float);
  j++;
  memcpy(buffer + j, &Tmp, sizeof(float));
  j += sizeof(float);

  mqttClient.publish("server", buffer, j, false);
#endif
}

void apiQuake() {
  memset(buffer, 0, BUFFER_SIZE);
  byte j = 0;
  buffer[j] = API_QUAKE;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  mqttClient.publish("server", buffer, j, false);
}

void apiTimeReq() {
  byte j = 0;
  buffer[j] = API_TIMEREQ;
  j++;

  // Device ID
  buffer[j] = 12;
  j++;
  getDeviceId(buffer + j);
  j += 12;

  mqttClient.publish("server", buffer, j, false);
}


// LAN discovery
EthernetUDP cmdsock;

void commandInterfaceInit() {
  cmdsock.begin(62001);
}

void commandInterfaceTick() {
  int packetSize = cmdsock.parsePacket();
  if (cmdsock.available()) {

    // read the packet into packetBufffer
    cmdsock.read(buffer, BUFFER_SIZE);

    if (memcmp("INGV\0", buffer, 5) != 0 || buffer[5] != PKTTYPE_DISCOVERY) {
      return;
    }

    // Reply to discovery
    buffer[5] = PKTTYPE_DISCOVERY_REPLY;

    memcpy(buffer + 6, ethernetMac, 6);

    memcpy(buffer + 12, VERSION, min(strlen(VERSION), 4));
    memcpy(buffer + 16, MODEL, min(strlen(MODEL), 8));

    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(buffer, 24);
    cmdsock.endPacket();
    cmdsock.flush();
  }
}

