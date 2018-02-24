
#ifndef __API_H
#define __API_H

#include "common.h"

// MQTT APIs
#define API_KEEPALIVE   1
#define API_QUAKE       2
#define API_TIMEREQ     3
#define API_TIMERESP    4
#define API_CFG         5
#define API_DISCONNECT  6
#define API_TEMPERATURE 7
#define API_REBOOT      8

boolean apiConnect();
void apiAlive();
void apiQuake();
void apiTimeReq();
unsigned long getUNIXTime();
void apiTick();
void apiDisconnect();


// LAN discovery APIs
#define CMD_INTERFACE_PORT  62001

#define PKTTYPE_DISCOVERY        1
#define PKTTYPE_DISCOVERY_REPLY  2

void commandInterfaceInit();
void commandInterfaceTick();


#endif
