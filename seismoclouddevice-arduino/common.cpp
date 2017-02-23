
#include "common.h"

byte ethernetMac[6] = { 0 };
char deviceid[13] = { 0 };

unsigned long bootTime = 0;

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
    /*byte lon[4];
    for (int i = 0; i < 4; i++) {
      lon[i] = EEPROM.read(22+i);
    }
    memcpy(&longitude, lon, 4);
    
    // Latitude
    byte lat[4];
    for (int i = 0; i < 4; i++) {
      lat[i] = EEPROM.read(26+i);
    }
    memcpy(&latitude, lat, 4);*/

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

unsigned long getBootTime() {
  return bootTime;
}

void setBootTime(unsigned long bootTimeMs) {
  bootTime = bootTimeMs;
}

#ifdef DEBUG
uint32_t probeSpeedStat = 0;

void setProbeSpeedStatistic(uint32_t v) {
  probeSpeedStat = v;
}

uint32_t getProbeSpeedStatistic() {
  return probeSpeedStat;
}
#endif

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


void reverse4bytes(byte* memory) {
  byte val[4];
  memcpy(&val, memory, 4);
  memcpy(memory+0, &val[3], 1);
  memcpy(memory+1, &val[2], 1);
  memcpy(memory+2, &val[1], 1);
  memcpy(memory+3, &val[0], 1);
}

/*
  time.c - low level time and date functions
  Copyright (c) Michael Margolis 2009-2014
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  1.0  6  Jan 2010 - initial release
  1.1  12 Feb 2010 - fixed leap year calculation error
  1.2  1  Nov 2010 - fixed setTime bug (thanks to Korman for this)
  1.3  24 Mar 2012 - many edits by Paul Stoffregen: fixed timeStatus() to update
                     status, updated examples for Arduino 1.0, fixed ARM
                     compatibility issues, added TimeArduinoDue and TimeTeensy3
                     examples, add error checking and messages to RTC examples,
                     add examples to DS1307RTC library.
  1.4  5  Sep 2014 - compatibility with Arduino 1.5.7
*/

#define LEAP_YEAR(Y) ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0

void printUNIXTime() {
  unsigned long unixTime = getUNIXTime();

  uint8_t year;
  uint8_t month, monthLength;
  unsigned long days;

  uint32_t time = (uint32_t)unixTime;
  uint8_t second = time % 60;
  time /= 60;
  
  uint8_t minute = time % 60;
  time /= 60;
  
  uint8_t hour = time % 24;
  time /= 24;
  
  uint8_t dayOfWeek = ((time + 4) % 7) + 1;
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
        break;
    }
  }
  uint8_t smonth = month + 1;  // jan is month 1  
  uint8_t day = time + 1;     // day of month

  char buf[50];
  memset(buf, 0, 50);
  snprintf(buf, 50, "%04i-%02i-%02i %02i:%02i:%02i UTC", year+1970, smonth, day, hour, minute, second);
  Serial.print(buf);
}


/* Copyright (C) 2012 by Victor Aprea <victor.aprea@wickeddevice.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include <avr/wdt.h>

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
// Function Implementation
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}

