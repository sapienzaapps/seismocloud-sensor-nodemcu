
#include "api.h"

EthernetClient ethernetClient;
PubSubClient mqttClient(ethernetClient);

#define API_BUFFER_SIZE 100
char apiBuffer[API_BUFFER_SIZE];
unsigned long lastNTPTime = 0;
unsigned long lastNTPMillis = 0;

void apiCallback(char* topic, byte* payload, unsigned int len) {
  if (len == 0) {
    return;
  }

  switch (payload[0]) {
    case API_TIMERESP:
      memset(apiBuffer, 0, API_BUFFER_SIZE);
      memcpy(&apiBuffer, payload+2, payload[1]);
      lastNTPTime = atol(apiBuffer);
      Serial.println(lastNTPTime);
      lastNTPMillis = millis();
      break;
    case API_CFG:
      /**
       * Payload (after type):
       * Offset       Byte     Desc
       * 1            4        Sigma (IEEE 754)
       */
      double sigma;
      memset(&sigma, payload+1, 4);
      setSigmaIter(sigma);
      Serial.print(F("Setting sigma to "));
      Serial.println(sigma);
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
  mqttClient.setServer("ingv-mqtt.netsplit.it", 61883);
  mqttClient.setCallback(apiCallback);
  mqttClient.connect(getDeviceId(), "test1", "test1");
  
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
    snprintf(apiBuffer, API_BUFFER_SIZE, "device/%s", getDeviceId());
    mqttClient.subscribe(apiBuffer);
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
/*
  char buf[10];
  bool pex = readParameter(reply, "sigma", buf, 10);
  if(pex) {
    setSigmaIter(atoi(buf));
  } else {
    setSigmaIter(getSigmaIter());
  }
  resetLastPeriod();
*/
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
  strcpy(apiBuffer+j, getDeviceId());
  j += strlen(getDeviceId());

  // Model
  apiBuffer[j] = 3;
  j++;
  strcpy(apiBuffer+j, "uno");
  j += 3;

  // Version
  apiBuffer[j] = strlen(VERSION);
  j++;
  strcpy(apiBuffer+j, VERSION);
  j += strlen(VERSION);

  // Lat&lon
  /*float l = getLatitude();
  memcpy(apiBuffer+j, &l, 4);
  j += 4;
  l = getLongitude();
  memcpy(apiBuffer+j, &l, 4);
  j += 4;*/
  
  mqttClient.publish("server", apiBuffer, j);
}

void apiQuake() {
  memset(apiBuffer, 0, API_BUFFER_SIZE);
  byte j = 0;
  apiBuffer[j] = API_QUAKE;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  strcpy(apiBuffer+j, getDeviceId());
  j += strlen(getDeviceId());

  // Lat&lon
  /*float l = getLatitude();
  memcpy(apiBuffer+j, &l, 4);
  j += 4;
  l = getLongitude();
  memcpy(apiBuffer+j, &l, 4);
  j += 4;*/
  
  mqttClient.publish("server", apiBuffer, j);
}

void apiTimeReq() {
  byte j = 0;
  apiBuffer[j] = API_TIMEREQ;
  j++;

  // Device ID
  apiBuffer[j] = strlen(getDeviceId());
  j++;
  strcpy(apiBuffer+j, getDeviceId());
  j += strlen(getDeviceId());
  
  mqttClient.publish("server", apiBuffer, j);
}

