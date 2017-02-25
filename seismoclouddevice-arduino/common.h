#ifndef __COMMON_H
#define __COMMON_H

/** 
 *  Configurazione utente: qui è possibile personalizzare alcuni aspetti
 *  User configuration: here you can customize some things
 */

#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5

#define DEBUG

/**
 * Fine parte configurabile
 * End config section
 */

#include <Arduino.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

#ifndef ESP8266
#define IS_ARDUINO
#else
#define IS_ESP
#endif

#ifdef IS_ARDUINO
#include "AcceleroMMA7361.h"
#else
#include "MPU6050.h"
#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#endif

#include "LED.h"
#include "CommandInterface.h"
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
