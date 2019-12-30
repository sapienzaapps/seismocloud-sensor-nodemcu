#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>

// ************************ BEGIN CONFIG **************************

// This flag will prevent updates - USE ONLY DURING DEVELOPMENT
// #define DONT_UPDATE

// On Arduino this flag has no effect if you use avr_boot bootloader (for self-update). You should switch to standard bootloader
// #define DEBUG

#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5

// ************************ END CONFIG **************************

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define VERSION     "1.30"
#define MODEL       "esp8266"

// ******* DEBUG PART
#ifdef DEBUG
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#else
#define Debug(x)
#define Debugln(x)
#endif
// ******* /DEBUG PART

#include <Wire.h>                // I2C
#include <ESP8266WiFi.h>         // ESP8266 Core WiFi Library
#include <WiFiUdp.h>             // WiFi UDP client Library
#include <DNSServer.h>           // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>    // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266httpUpdate.h>   // For OTA
#include "MPU6050.h"

#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#define soft_restart()      \
apiDisconnect();            \
delay(5000);                \
ESP.restart()

#ifdef DEBUG
#define MQTT_SEISMOCLOUD_HOST  "mqtt-seismocloud.test.sapienzaapps.it"
#define MQTT_SEISMOCLOUD_PORT  1883
#define UPDATE_SERVER          "firmware-seismocloud.test.sapienzaapps.it"
#else
#define MQTT_SEISMOCLOUD_HOST  "mqtt.seismocloud.com"
#define MQTT_SEISMOCLOUD_PORT  1883
#define UPDATE_SERVER          "firmware.seismocloud.com"
#endif

#include "update.h"
#include "LED.h"
#include "seismometer.h"
#include "api.h"
#include "lan-discovery.h"
#include "nodemcu.h"

#define BUFFER_SIZE 512
extern byte buffer[BUFFER_SIZE];
extern byte ethernetMac[6];
extern char deviceid[13];
extern bool streamingEnabled;
extern ushort probeSpeedHz;
extern uint32_t probeSpeedStat;
extern const char *tlspubkey PROGMEM;

/**
 * Force a MAC Address
 */
void setMACAddress(byte* mac);

#ifdef DEBUG
void printMACAddress();
void printUNIXTime();
#endif

#endif
