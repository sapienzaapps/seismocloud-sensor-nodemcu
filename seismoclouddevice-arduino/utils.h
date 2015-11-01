
#ifndef __UTILS_H
#define __UTILS_H

#include <Arduino.h>

String macToString(const byte* mac);
bool isZero(byte* buf, int bufsize);
void prepareUuid(byte* uuidNumber, char* buf);
void printUuid(byte* uuidNumber);
void printHex(byte number);

#endif
