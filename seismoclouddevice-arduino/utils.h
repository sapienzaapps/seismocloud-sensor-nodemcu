
#ifndef __UTILS_H
#define __UTILS_H

#include <Arduino.h>

String macToString(const byte* mac);
void reverse4bytes(byte* memory);
unsigned long tcpLatency();

#endif
