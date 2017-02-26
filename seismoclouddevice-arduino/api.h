
#ifndef __API_H
#define __API_H

#include "common.h"

// MQTT APIs
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


// LAN discovery APIs
#define CMD_INTERFACE_PORT  62001
#define PACKET_SIZE         50

typedef enum {
  PKTTYPE_DISCOVERY = 1,
  PKTTYPE_DISCOVERY_REPLY = 2
} PacketType;

void commandInterfaceInit();
void commandInterfaceTick();


#endif
