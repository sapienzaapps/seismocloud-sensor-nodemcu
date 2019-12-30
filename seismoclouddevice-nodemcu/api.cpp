
#include "api.h"

#define TOPIC_BUFFER_SIZE 60
WiFiClient ethernetClient;
PubSubClient mqttClient(ethernetClient);

uint64 lastNTPTime = 0;
unsigned long lastNTPMillis = 0;

char emptypayload[2] = { 'y' , 0 };
char topicbuffer[TOPIC_BUFFER_SIZE + 1] = { 0 };

void apiCallback(char* topic, byte* payload, unsigned int len);

boolean apiConnect() {
  // Will message for disconnection
  char willtopic[TOPIC_BUFFER_SIZE + 1] = { 0 };
  memset(willtopic, 0, TOPIC_BUFFER_SIZE);
  snprintf(willtopic, TOPIC_BUFFER_SIZE, "sensor/%s/disconnect", deviceid);
  // END Will message

  Debugln(F("[MQTT] Connecting"));
  mqttClient.setServer(MQTT_SEISMOCLOUD_HOST, MQTT_SEISMOCLOUD_PORT);
  mqttClient.setCallback(apiCallback);
  mqttClient.connect((char*)(deviceid), "embedded", "embedded", willtopic, 0, 0, emptypayload);

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
    Debugln("[MQTT] Subscribing to topics");
    
    memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
    snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/sigma", deviceid);
    mqttClient.subscribe((char*)topicbuffer, 0);
    
    memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
    snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/probespeed", deviceid);
    mqttClient.subscribe((char*)topicbuffer, 0);
    
    memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
    snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/reboot", deviceid);
    mqttClient.subscribe((char*)topicbuffer, 0);
    
    memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
    snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/timesync", deviceid);
    mqttClient.subscribe((char*)topicbuffer, 0);
    
    memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
    snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/stream", deviceid);
    mqttClient.subscribe((char*)topicbuffer, 0);
    
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
  uint64 t3 = getUNIXTimeMS();
  if (len == 0) {
    return;
  }

  // Skip the "sensor/<mysensorid>/" part in topic, and detect shorter topics
  if (strlen(topic) < 7 + strlen(deviceid) + 1) {
    return;
  }
  char *command = topic + 7 + strlen(deviceid) + 1;
  memset(buffer, 0, BUFFER_SIZE);
  memcpy(buffer, payload, min(len, BUFFER_SIZE-1));

  if (strcmp(command, "sigma") == 0) {
    float sigma;
    sscanf((char*)buffer, "%f", &sigma);
    setSigmaIter(sigma);
    resetLastPeriod();

    Debug(F("[MQTT] Setting sigma to "));
    Debugln(sigma);
  } else if (strcmp(command, "reboot") == 0) {
    apiDisconnect();
    soft_restart();
  } else if (strcmp(command, "timesync") == 0) {
    double t0, t1, t2;
    sscanf((char*)buffer, "%lf;%lf;%lf", &t0, &t1, &t2);
    
		lastNTPTime = t2 + ((t3 - t0) - (t2 - t1));
    lastNTPMillis = millis();

#ifdef DEBUG
    Debug(F("[MQTT] Time sync'ed: "));
    printUNIXTime();
#endif
  } else if (strcmp(command, "stream") == 0) {
    streamingEnabled = strncmp("on", (char*)buffer, len) == 0;
  } else if (strcmp(command, "probespeed") == 0) {
    sscanf((char*)buffer, "%hu", &probeSpeedHz);
    Debug(F("[MQTT] New probe speed: "));
    Debugln(probeSpeedHz);
  } else {
    // Unknown/unsupported command received
    Debug("[MQTT] Unsupported command received: ");
    Debugln(command);
  }
}

void apiAlive() {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf((char*)buffer, BUFFER_SIZE, "%s;%s", MODEL, VERSION);

  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/alive", deviceid);

  mqttClient.publish((char*)topicbuffer, (char*)buffer);
}

void apiQuake(double x, double y, double z) {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf((char*)buffer, BUFFER_SIZE, "%d;%f;%f;%f", getUNIXTimeMS(), x, y, z);

  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/quake", deviceid);

  mqttClient.publish((char*)topicbuffer, (char*)buffer);
}

void apiTimeReq() {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf((char*)buffer, BUFFER_SIZE, "%llu", lastNTPTime);

  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/timereq", deviceid);

  mqttClient.publish((char*)topicbuffer, (char*)buffer);
}

void apiTemperature(float temperature) {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf((char*)buffer, BUFFER_SIZE, "%f", temperature);

  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/temperature", deviceid);

  mqttClient.publish((const char*)topicbuffer, (const char*)buffer);
}

void apiStream(double x, double y, double z) {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf((char*)buffer, BUFFER_SIZE, "%lu000;%f;%f;%f", getUNIXTime(), x, y, z);

  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/streamdata", deviceid);

  mqttClient.publish((char*)topicbuffer, (char*)buffer);
}

void apiTick() {
  if (!mqttClient.loop()) {
    delay(3000);
    apiConnect();
  }
}

void apiDisconnect() {
  memset(topicbuffer, 0, TOPIC_BUFFER_SIZE);
  snprintf(topicbuffer, TOPIC_BUFFER_SIZE, "sensor/%s/disconnect", deviceid);
  mqttClient.publish((char*)topicbuffer, emptypayload);
  mqttClient.disconnect();
}

unsigned long getUNIXTime() {
  if (lastNTPTime == 0) {
    return 0;
  }
  unsigned long diffms = millis() - lastNTPMillis;
  return (lastNTPTime / 1000) + (diffms / 1000);
}

uint64_t getUNIXTimeMS() {
  if (lastNTPTime == 0) {
    return 0;
  }
  unsigned long diffms = millis() - lastNTPMillis;
  return lastNTPTime + diffms;
}
