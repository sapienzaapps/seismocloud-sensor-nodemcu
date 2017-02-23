#ifndef __COMMON_H
#define __COMMON_H

/** 
 *  Configurazione utente: qui è possibile personalizzare alcuni aspetti
 *  User configuration: here you can customize some things
 */

#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5

//#define DEBUG

/**
 * Fine parte configurabile
 * End config section
 */

#include <Arduino.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

#include "LED.h"
#include "AcceleroMMA7361.h"
#include "CommandInterface.h"
#include "seismometer.h"
#include "api.h"

void initEEPROM();
void checkEEPROM();
unsigned long getBootTime();
void setBootTime(unsigned long);
void getMACAddress(byte* mac);
const char* getDeviceId();
void loadConfig();

#include <avr/wdt.h>

#define soft_restart()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)

void reverse4bytes(byte* memory);
void printUNIXTime();

#ifdef DEBUG
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
#endif

#endif
