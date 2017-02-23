
#ifndef __API_H
#define __API_H

#include "common.h"

#define API_KEEPALIVE   1
#define API_QUAKE       2
#define API_TIMEREQ     3
#define API_TIMERESP    4
#define API_CFG         5

boolean apiConnect();
void apiAlive();
void apiQuake();
void apiTimeReq();
unsigned long getUNIXTime();
void apiTick();


#endif
