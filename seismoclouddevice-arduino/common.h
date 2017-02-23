
#ifndef __COMMON_H
#define __COMMON_H

#include <Ethernet.h>
#include <EEPROM.h>
#include <PubSubClient.h>

#define VERSION     "1.20"
// #define RESET_ENABLED

#include "LED.h"
#include "AcceleroMMA7361.h"
#include "utils.h"
#include "CommandInterface.h"
#include "seismometer.h"
#include "api.h"

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
const char* getDeviceId();
void loadConfig();
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
String getVersionAsString();

#endif
