
#ifndef __COMMON_H
#define __COMMON_H

#include <Ethernet.h>
#include <EEPROM.h>

#include "LED.h"
#include "TrueRandom.h"
#include "SoftReset.h"
#include "AcceleroMMA7361.h"
#include "ntp.h"
#include "utils.h"
#include "httpclient.h"
#include "CommandInterface.h"
#include "seismometer.h"
#include "MemoryFree.h"

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
void generateMACAddress(byte* mac);
String getVersionAsString();
bool readParameter(char* cfg, char* tag, char* into, int maxn);

#endif
