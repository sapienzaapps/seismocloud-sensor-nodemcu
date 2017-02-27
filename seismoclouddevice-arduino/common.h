#ifndef __COMMON_H
#define __COMMON_H

#include <Arduino.h>
#ifdef ARDUINO_AVR_UNO
#define IS_ARDUINO
#define MODEL "uno"
#else

#ifdef ESP8266
#define IS_ESP
#define MODEL "esp8266"
#else

#ifdef ARDUINO_AVR_MEGA2560
#define IS_ARDUINO
#define MODEL "mega2560"
#else

#ifdef ARDUINO_AVR_ETHERNET
#define IS_ARDUINO
#define MODEL "uno-e"
#else

#error "Unsupported platform"
#endif
#endif
#endif
#endif

// #define DEBUG

#define DONT_UPDATE

#ifdef IS_ARDUINO
// Configurazione LED per Arduino
#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5
#else
#ifdef IS_ESP
// Configurazione LED per NodeMCU/ESP8266
#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5
#endif
#endif

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

#ifndef DONT_UPDATE
#ifdef IS_ARDUINO
#include <SD.h>
#endif
#include "update.h"
#endif

#ifdef IS_ARDUINO
#include "AcceleroMMA7361.h"
#include <avr/wdt.h>

#define soft_restart()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)
#endif

#ifdef IS_ESP
#endif

#ifdef IS_ESP
#include <Wire.h>                // I2C
#include <ESP8266WiFi.h>         // ESP8266 Core WiFi Library
#include <DNSServer.h>           // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>    // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266httpUpdate.h>   // For OTA
#include "MPU6050.h"

#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#define soft_restart() ESP.restart()
#endif

#include "LED.h"
#include "seismometer.h"
#include "api.h"
#include "nodemcu.h"

#define BUFFER_SIZE 50
extern byte buffer[BUFFER_SIZE];
extern byte ethernetMac[6];

void initEEPROM();
void checkEEPROM();
void checkMACAddress();
void setMACAddress(byte* mac);
void printMACAddress();
void loadConfig();
void getDeviceId(char* dest);
void getDeviceId(byte* dest);

#ifdef DEBUG
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
