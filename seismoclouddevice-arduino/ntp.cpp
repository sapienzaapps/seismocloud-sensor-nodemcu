/*
 * © Francesco Potortì 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */

#include "common.h"

unsigned long ntpUnixTime ()
{
  EthernetUDP udp;
  static int udpInited = udp.begin(123); // open socket on arbitrary port

  const char timeServer[] = "pool.ntp.org";  // NTP server

  // Only the first four bytes of an outgoing NTP packet need to be set
  // appropriately, the rest can be whatever.
  const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

  // Fail if WiFiUdp.begin() could not init a socket
  if (! udpInited) {
    return 0;
  }

  // Clear received data from possible stray received packets
  udp.flush();

  // Send an NTP request
  if (! (udp.beginPacket(timeServer, 123) // 123 is the NTP port
   && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
   && udp.endPacket())) {
    return 0;       // sending request failed
  }
  
  // Wait for response; check every pollIntv ms up to maxPoll times
  const int pollIntv = 50;   // poll every this many ms
  const byte maxPoll = 100;    // poll up to this many times
  int pktLen;       // received packet length
  for (byte i=0; i<maxPoll; i++) {
    if ((pktLen = udp.parsePacket()) == 48)
      break;
    delay(pollIntv);
  }
  if (pktLen != 48) {
    return 0;       // no correct packet received
  }
  
  // Read and discard the first useless bytes
  // Set useless to 32 for speed; set to 40 for accuracy.
  const byte useless = 40;
  for (byte i = 0; i < useless; ++i)
    udp.read();

  // Read the integer part of sending time
  unsigned long time = udp.read();  // NTP time
  for (byte i = 1; i < 4; i++)
    time = time << 8 | udp.read();

  // Round to the nearest second if we want accuracy
  // The fractionary part is the next byte divided by 256: if it is
  // greater than 500ms we round to the next second; we also account
  // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
  // additionally, we account for how much we delayed reading the packet
  // since its arrival, which we assume on average to be pollIntv/2.
  time += (udp.read() > 115 - pollIntv/8);

  // Discard the rest of the packet
  udp.flush();

  return time - 2208988800ul;   // convert NTP time to Unix time
}

unsigned long lastNTPTime = 0;
unsigned long lastNTPMillis = 0;

unsigned long updateNTP() {
  if(lastNTPMillis == 0 || millis() - lastNTPMillis > 1000 * 60 * 5) {
    unsigned long ntpTime = ntpUnixTime();
    if(ntpTime != 0) {
      Serial.print(F("NTP time updated, clock skew: "));
      Serial.println(getUNIXTime() - ntpTime);
      lastNTPTime = ntpTime;
      lastNTPMillis = millis();
    }
  }
}

unsigned long getUNIXTime() {
  if(lastNTPTime == 0) {
    return 0;
  }
  unsigned long diffms = millis() - lastNTPMillis;
  return lastNTPTime + (diffms/1000);
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

