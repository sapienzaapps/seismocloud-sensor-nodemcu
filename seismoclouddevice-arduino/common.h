
#ifndef __COMMON_H
#define __COMMON_H

#include <Ethernet.h>
#include <EEPROM.h>

#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5
#define VERSION     "1.10"

//#define RESET_ENABLED

#include "LED.h"
#include "SoftReset.h"
#include "AcceleroMMA7361.h"
#include "ntp.h"
#include "utils.h"
#include "httpclient.h"
#include "CommandInterface.h"
#include "seismometer.h"
#include "MemoryFree.h"

void initEEPROM();
void checkEEPROM();
float getLatitude();
String getLatitudeAsString();
String getLongitudeAsString();
float getLongitude();
unsigned long getBootTime();
void setBootTime(unsigned long);
void setLatitude(float l);
void setLongitude(float l);
void getMACAddress(byte* mac);
void loadConfig();
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
String getVersionAsString();

#endif
