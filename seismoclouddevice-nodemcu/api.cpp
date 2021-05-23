
#include "api.h"
#include <WiFiClientSecure.h>

PubSubClient mqttClient;
WiFiClient client;

void apiCallback(char* topic, byte* payload, unsigned int len);

boolean apiConnect() {

  Debugln(F("[MQTT] Connecting"));
  mqttClient.setServer("192.168.151.110", 1883);
  mqttClient.setClient(client);
  mqttClient.setCallback(apiCallback);
  mqttClient.connect((char*)(deviceid));

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
    Debugln("[MQTT] Done");
    return true;
  } else {
    return false;
  }
}

/**
 * MQTT Callback (executed when a MQTT message is received)
 */
void apiCallback(char* topic, byte* payload, unsigned int len) {
}

void apiStream() {
  snprintf((char*)buffer, BUFFER_SIZE, "%f;%f;%f", acceleroX, acceleroY, acceleroZ);
  mqttClient.publish("stream", (char*)buffer);
}

void apiTick() {
  // Do MQTT loop and check for MQTT connection
  if (!mqttClient.loop()) {
    LED_lost_api();
    Debugln(F("[MQTT] Connection lost, retrying in 3 seconds"));
    delay(3000);

    if (!apiConnect()) {
      Debugln(F("[MQTT] Connection failed, rebooting"));
      soft_restart();
    }
    LED_restore_api();
  }
}

void apiDisconnect() {
  mqttClient.disconnect();
}
