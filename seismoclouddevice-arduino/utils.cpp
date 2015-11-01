
#include "common.h"

String macToString(const byte* mac) {
  char buf[13];
  memset(buf, 0, 13);
  snprintf(buf, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

bool isZero(byte* buf, int bufsize) {
  bool ret = true;
  for(int i=0; i < bufsize && ret; i++) {
    if(buf[i] != 0) {
      ret = false;
    }
  }
  return ret;
}

void prepareUuid(byte* uuidNumber, char* buf) {
  int i,bufpos=0;
  for (i=0; i<16; i++) {
    if (i==4 || i==6 || i==8 || i==10) {
      buf[bufpos] = '-';
      bufpos++;
    }
    snprintf(buf+bufpos, 3, "%02.2x", uuidNumber[i]);
    bufpos += 2;
  }
}

void printUuid(byte* uuidNumber) {
  int i;
  for (i=0; i<16; i++) {
    if (i==4 || i==6 || i==8 || i==10) Serial.print("-");
    printHex(uuidNumber[i]);
  }
}

void printHex(byte number) {
  int topDigit = number >> 4;
  int bottomDigit = number & 0x0f;
  // Print high hex digit
  Serial.print( "0123456789ABCDEF"[topDigit] );
  // Low hex digit
  Serial.print( "0123456789ABCDEF"[bottomDigit] );
}


