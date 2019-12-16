
#ifndef __LAN_DISCOVERY_H
#define __LAN_DISCOVERY_H

#include "common.h"

// LAN discovery APIs
#define CMD_INTERFACE_PORT  62001

#define PKTTYPE_DISCOVERY        1
#define PKTTYPE_DISCOVERY_REPLY  2

/**
 * LAN discovery init
 */
void commandInterfaceInit();

/**
 * Maintain LAN discovery process
 * You should call this function at each loop() function execution
 */
void commandInterfaceTick();

#endif
