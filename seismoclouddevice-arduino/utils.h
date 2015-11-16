
#ifndef __UTILS_H
#define __UTILS_H

#include <Arduino.h>

String macToString(const byte* mac);
bool isZero(byte* buf, int bufsize);
void reverse4bytes(byte* memory);

#endif
