
#include "common.h"

#define EEPROM_BASE_ADDR 0

// UUID
byte uuidNumber[16] = { 0 };

float latitude = 0;
float longitude = 0;
unsigned long bootTime = 0;
uint32_t probeSpeedStat = 0;

bool checkEEPROM() {
  return EEPROM.read(0) == 'S'
    && EEPROM.read(1) == 'E'
    && EEPROM.read(2) == 'I'
    && EEPROM.read(3) == 'S'
    && EEPROM.read(4) == 'M'
    && EEPROM.read(5) == 'O';
}

void initEEPROM() {
  EEPROM.write(0, 'S');
  EEPROM.write(1, 'E');
  EEPROM.write(2, 'I');
  EEPROM.write(3, 'S');
  EEPROM.write(4, 'M');
  EEPROM.write(5, 'O');

  for (int i = 6 ; i < 6+16+4+4; i++) {
    EEPROM.write(i, 0);
  }
}

void loadConfig() {
  bool cfg = checkEEPROM();
  if(!cfg) {
    initEEPROM();
  } else {
    
    // Load UUID
    for (int i = 0; i < 16; i++) {
      uuidNumber[i] = EEPROM.read(i+6);
    }

    // Longitude
    byte lon[4];
    for (int i = 0; i < 4; i++) {
      lon[i] = EEPROM.read(22+i);
    }
    memcpy(&longitude, lon, 4);
    
    // Latitude
    byte lat[4];
    for (int i = 0; i < 4; i++) {
      lat[i] = EEPROM.read(26+i);
    }
    memcpy(&latitude, lat, 4);
    
  }
}

byte* getUuidNumber() {
  return uuidNumber;
}

void setUuidNumber(byte* uuid) {
  memcpy(uuidNumber, uuid, 16);
  
  // Save UUID
  for (int i = 0; i < 16; i++) {
    EEPROM.write(i+6, uuidNumber[i]);
  }
}

float getLatitude() {
  return latitude;
}

void setLatitude(float l) {
  latitude = l;
  
  // Latitude
  byte lat[4];
  memcpy(lat, &latitude, 4);
  
  for (int i = 0; i < 4; i++) {
    EEPROM.write(26+i, lat[i]);
  }
}

void setLongitude(float l) {
  longitude = l;

  // Longitude
  byte lon[4];
  memcpy(lon, &longitude, 4);
  for (int i = 0; i < 4; i++) {
    EEPROM.write(22+i, lon[i]);
  }
}

float getLongitude() {
  return longitude;
}

unsigned long getBootTime() {
  return bootTime;
}

void setBootTime(unsigned long bootTimeMs) {
  bootTime = bootTimeMs;
}


void setProbeSpeedStatistic(uint32_t v) {
  probeSpeedStat = v;
}

uint32_t getProbeSpeedStatistic() {
  return probeSpeedStat;
}



