#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>

#ifdef ESP8266
#define IS_ESP
#define MODEL "esp8266"
#else

#endif

// #define DONT_UPDATE
// #define DEBUG

#ifdef IS_ESP
// Configurazione LED per NodeMCU/ESP8266
#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5
#endif

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

#ifdef DEBUG
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#else
#define Debug(x)
#define Debugln(x)
#endif

#ifndef DONT_UPDATE

#include "update.h"
#endif

#ifdef IS_ESP
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
#endif

#include "LED.h"
#include "seismometer.h"
#include "api.h"
#include "nodemcu.h"

#define BUFFER_SIZE 512
extern byte buffer[BUFFER_SIZE];
extern byte ethernetMac[6];

void initEEPROM();
void checkEEPROM();
void checkMACAddress();
void setMACAddress(byte* mac);
void loadConfig();
void getDeviceId(char* dest);
void getDeviceId(byte* dest);
void selectSD();
void selectEthernet();

#ifdef DEBUG
void printMACAddress();
void printUNIXTime();
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
#endif

class MyRingBuffer
{
public:
  MyRingBuffer(unsigned int size);
  ~MyRingBuffer();

  void reset();
  void init();
  void push(char c);
  int getPos();
  bool endsWith(const char* str);

  unsigned int _size;
  char* ringBuf;
  char* ringBufEnd;
  char* ringBufP;

};

#endif
