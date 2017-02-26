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

#define DEBUG

#ifdef IS_ARDUINO
// Configurazione LED per Arduino
#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5
#else
#ifdef IS_ESP
// Configurazione LED per NodeMCU/ESP8266
#define LED_RED     D0
#define LED_YELLOW  D3
#define LED_GREEN   D4
#endif
#endif

#include <Ethernet.h>
#include <EEPROM.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

#ifdef IS_ARDUINO
#include "AcceleroMMA7361.h"
#else
#include "MPU6050.h"
#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#endif

#include "LED.h"
#include "seismometer.h"
#include "api.h"
#include "nodemcu.h"

void initEEPROM();
void checkEEPROM();
unsigned long getBootTime();
void setBootTime(unsigned long);
void getMACAddress(byte* mac);
void setMACAddress(byte* mac);
const char* getDeviceId();
void loadConfig();

#ifdef IS_ARDUINO
#include <avr/wdt.h>

#define soft_restart()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)
#else
#ifdef IS_ESP
#define soft_restart() ESP.restart()
#else
#error "Unknown platform - reboot"
#endif
#endif

void reverse4bytes(byte* memory);
void printUNIXTime();

#ifdef DEBUG
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
#endif

#endif
