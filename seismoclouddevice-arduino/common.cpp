
#include "common.h"

byte ethernetMac[6] = { 0 };
char deviceid[13] = { 0 };

float latitude = 0;
float longitude = 0;
unsigned long bootTime = 0;
uint32_t probeSpeedStat = 0;

void checkEEPROM() {
  EEPROM.write(0, 'S');
  EEPROM.write(1, 'E');

  if(!EEPROM.read(0) == 'S' || !EEPROM.read(1) == 'E') {
    Serial.println(F("EEPROM failed"));
    while(true) {
      LED::green(false);
      LED::red(true);
      LED::yellow(true);
      delay(400);
    }
  }
}

bool validateEEPROM() {
  return EEPROM.read(0) == 'S'
    && EEPROM.read(1) == 'E'
    && EEPROM.read(2) == 'I'
    && EEPROM.read(3) == 'S'
    && EEPROM.read(4) == 'M'
    && EEPROM.read(5) == 'O';
}

void initEEPROM() {
  LED::green(false);
  LED::red(true);
  LED::yellow(false);
  
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  
  EEPROM.write(0, 'S');
  EEPROM.write(1, 'E');
  EEPROM.write(2, 'I');
  EEPROM.write(3, 'S');
  EEPROM.write(4, 'M');
  EEPROM.write(5, 'O');
  
  LED::red(false);
}

void _updateMacStr() {
  memset(deviceid, 0, 13);
  snprintf(deviceid, 12, "%02x%02x%02x%02x%02x%02x", ethernetMac[0], ethernetMac[1], ethernetMac[2], ethernetMac[3], ethernetMac[4], ethernetMac[5]);
}

void loadConfig() {
  bool cfg = validateEEPROM();
  if(!cfg) {
    initEEPROM();
  } else {
    
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

    // Load MAC Address
    for (int i = 0; i < 6; i++) {
      ethernetMac[i] = EEPROM.read(30+i);
    }
    _updateMacStr();
  }
}

void _saveMACAddress() {
  for (int i = 0; i < 6; i++) {
    EEPROM.write(30+i, ethernetMac[i]);
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

void generateMACAddress() {
  randomSeed(analogRead(A5));
  
  *(ethernetMac+0) = 0x06; // LAA
  *(ethernetMac+1) = (byte)random(0, 255);
  *(ethernetMac+2) = (byte)random(0, 255);
  *(ethernetMac+3) = (byte)random(0, 255);
  *(ethernetMac+4) = (byte)random(0, 255);
  *(ethernetMac+5) = (byte)random(0, 255);
  _saveMACAddress();
}

void getMACAddress(byte* mac) {
  if (ethernetMac[0] == 0) {
    generateMACAddress();
    _updateMacStr();
  }
  memcpy(mac, ethernetMac, 6);
}

const char* getDeviceId() {
  return deviceid;
}

void ftoa(char* buf, int m, float fnum) {
  int num = (int)fnum;
  fnum -= num;
  int decimals = (int)(fnum*10000.0);
  snprintf(buf, m, "%i.%04i", num, decimals);
}

String getLatitudeAsString() {
  char v[11];
  memset(v, 0, 11);
  ftoa(v, 10, latitude);
  return String(v);
}

String getLongitudeAsString() {
  char v[10+1];
  memset(v, 0, 10+1);
  ftoa(v, 10, longitude);
  return String(v);
}

String getVersionAsString() {
  return String(VERSION);
}

