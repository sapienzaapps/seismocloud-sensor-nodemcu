
#ifndef __API_H
#define __API_H

#include "common.h"

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
 * Send the stream to API server
 */
void apiStream();

/**
 * Maintain API server connection
 * You should call this function at each loop() function execution
 */
void apiTick();

/**
 * Disconnect from API server
 */
void apiDisconnect();

/**
 * Returns current UNIX Time
 */
unsigned long getUNIXTime();


/**
 * Returns current UNIX Time in milliseconds
 */
uint64_t getUNIXTimeMS();

#endif
