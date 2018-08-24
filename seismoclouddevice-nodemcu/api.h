
#ifndef __API_H
#define __API_H

#include "common.h"

// API commands
#define API_KEEPALIVE   1
#define API_QUAKE       2
#define API_TIMEREQ     3
#define API_TIMERESP    4
#define API_CFG         5
#define API_DISCONNECT  6
#define API_TEMPERATURE 7
#define API_REBOOT      8

/**
 * Connect to API server
 */
boolean apiConnect();

/**
 * Send an ALIVE event to API server
 * You should call this function regularly (every 15 minutes)
 */
void apiAlive();

/**
 * Send a QUAKE event to API server
 */
void apiQuake();

/**
 * Request a time update to API server
 */
void apiTimeReq();

/**
 * Returns current UNIX Time
 */
unsigned long getUNIXTime();

/**
 * Maintain API server connection
 * You should call this function at each loop() function execution
 */
void apiTick();

/**
 * Disconnect from API server
 */
void apiDisconnect();


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
