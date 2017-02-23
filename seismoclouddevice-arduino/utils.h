
#ifndef __UTILS_H
#define __UTILS_H

#include <Arduino.h>
#include <avr/wdt.h>

#define soft_restart()        \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)


// MemoryFree library based on code posted here:
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1213583720/15
// Extended by Matthew Murdoch to include walking of the free list.
#ifdef __cplusplus
extern "C" {
#endif

int freeMemory();

#ifdef  __cplusplus
}
#endif

void reverse4bytes(byte* memory);
unsigned long tcpLatency();
void printUNIXTime();

#endif
