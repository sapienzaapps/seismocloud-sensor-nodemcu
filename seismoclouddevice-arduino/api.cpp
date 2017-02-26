
#include "api.h"

#ifdef IS_ARDUINO
EthernetClient ethernetClient;
#endif
#ifdef IS_ESP
WiFiClient ethernetClient;
#endif
PubSubClient mqttClient(ethernetClient);

#define API_BUFFER_SIZE 100
byte apiBuffer[API_BUFFER_SIZE];
unsigned long lastNTPTime = 0;
unsigned long lastNTPMillis = 0;

void apiCallback(char* topic, byte* payload, unsigned int len) {
  if (len == 0) {
    return;
  }

  switch (payload[0]) {
    case API_TIMERESP:
      memcpy(&lastNTPTime, payload+1, 4);
      Serial.println(lastNTPTime);
      lastNTPMillis = millis();
      break;
    case API_CFG:
      /**
       * Payload (after type):
       * Offset       Byte     Desc
       * 1            4        Sigma (IEEE 754)
       */
      float sigma;
      memcpy(&sigma, payload+1, 4);
      setSigmaIter(sigma);
      Serial.print(F("Setting sigma to "));
      Serial.println(sigma);
      resetLastPeriod();
      break;
    default:
      break;
  }
}

unsigned long getUNIXTime() {
  if(lastNTPTime == 0) {
    return 0;
  }
  unsigned long diffms = millis() - lastNTPMillis;
  return lastNTPTime + (diffms/1000);
}

boolean apiConnect() {
  // Will message for disconnection
  memset(apiBuffer, 0, API_BUFFER_SIZE);
  byte j = 0;
  apiBuffer[j] = API_DISCONNECT;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  memcpy(apiBuffer+j, getDeviceId(), strlen(getDeviceId()));
  j += strlen(getDeviceId());

  // END Will message
  
  mqttClient.setServer("ingv-mqtt.netsplit.it", 61883);
  mqttClient.setCallback(apiCallback);
  mqttClient.connect(getDeviceId(), "test1", "test1", "server", 0, 0, (char*)apiBuffer);
  
#ifdef DEBUG
    switch(mqttClient.state()) {
      case MQTT_CONNECTION_TIMEOUT:
        Serial.println(F("[MQTT] Timeout"));
        break;
      case MQTT_CONNECTION_LOST:
        Serial.println(F("[MQTT] Connection lost"));
        break;
      case MQTT_CONNECT_FAILED:
        Serial.println(F("[MQTT] Connection failed"));
        break;
      case MQTT_DISCONNECTED:
        Serial.println(F("[MQTT] Disconnected"));
        break;
      case MQTT_CONNECTED:
        Serial.println(F("[MQTT] Connected"));
        break;
      case MQTT_CONNECT_BAD_PROTOCOL:
        Serial.println(F("[MQTT] Bad protocol"));
        break;
      case MQTT_CONNECT_BAD_CLIENT_ID:
        Serial.println(F("[MQTT] Bad client ID"));
        break;
      case MQTT_CONNECT_UNAVAILABLE:
        Serial.println(F("[MQTT] Server unavailable"));
        break;
      case MQTT_CONNECT_BAD_CREDENTIALS:
        Serial.println(F("[MQTT] Bad credentials"));
        break;
      case MQTT_CONNECT_UNAUTHORIZED:
        Serial.println(F("[MQTT] Unauthorized"));
        break;
    }
#endif
  
  if (mqttClient.state() == MQTT_CONNECTED) {
    memset(apiBuffer, 0, API_BUFFER_SIZE);
    snprintf((char*)apiBuffer, API_BUFFER_SIZE, "device/%s", getDeviceId());
    mqttClient.subscribe((char*)apiBuffer);
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
   * Keep alive message format:
   * Position   Size      Value
   * 0          1         1 (type=alive)
   * 1          1         device id length
   * 2          len(did)  device id as ASCII string
   * #          1         device model length
   * #          len(mod)  device model
   * #          1         version length
   * #          len(ver)  version as ASCII string
   * 
   */
  memset(apiBuffer, 0, API_BUFFER_SIZE);
  byte j = 0;
  apiBuffer[j] = API_KEEPALIVE;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  memcpy(apiBuffer+j, getDeviceId(), strlen(getDeviceId()));
  j += strlen(getDeviceId());

  // Model
  apiBuffer[j] = strlen(MODEL);
  j++;
  memcpy(apiBuffer+j, MODEL, strlen(MODEL));
  j += 3;

  // Version
  apiBuffer[j] = strlen(VERSION);
  j++;
  memcpy(apiBuffer+j, VERSION, strlen(VERSION));
  j += strlen(VERSION);
  
  mqttClient.publish("server", apiBuffer, j, false);
}

void apiQuake() {
  memset(apiBuffer, 0, API_BUFFER_SIZE);
  byte j = 0;
  apiBuffer[j] = API_QUAKE;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  memcpy(apiBuffer+j, getDeviceId(), strlen(getDeviceId()));
  j += strlen(getDeviceId());
  
  mqttClient.publish("server", apiBuffer, j, false);
}

void apiTimeReq() {
  byte j = 0;
  apiBuffer[j] = API_TIMEREQ;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  memcpy(apiBuffer+j, getDeviceId(), strlen(getDeviceId()));
  j += strlen(getDeviceId());
  
  mqttClient.publish("server", apiBuffer, j, false);
}


// LAN discovery
byte udpPacketBuffer[PACKET_SIZE];
EthernetUDP cmdsock;

void commandInterfaceInit() {
  cmdsock.begin(62001);
}

void commandInterfaceTick() {
  int packetSize = cmdsock.parsePacket();
  if(cmdsock.available()) {
    
    // read the packet into packetBufffer
    cmdsock.read(udpPacketBuffer, PACKET_SIZE);

    if(memcmp("INGV\0", udpPacketBuffer, 5) != 0 || udpPacketBuffer[5] != PKTTYPE_DISCOVERY) {
      return;
    }

    byte macaddress[6] = { 0 };
    getMACAddress(macaddress);

    // Reply to discovery
    udpPacketBuffer[5] = PKTTYPE_DISCOVERY_REPLY;

    memcpy(udpPacketBuffer + 6, macaddress, 6);
    
    memcpy(udpPacketBuffer + 12, VERSION, min(strlen(VERSION), 4));
    memcpy(udpPacketBuffer + 16, MODEL, min(strlen(MODEL), 8));
    
    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(udpPacketBuffer, PACKET_SIZE);
    cmdsock.endPacket();
    cmdsock.flush();
  }
}

