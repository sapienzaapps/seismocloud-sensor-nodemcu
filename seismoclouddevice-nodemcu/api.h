
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
void apiQuake(double x, double y, double z);

/**
 * Request a time update to API server
 */
void apiTimeReq();

/**
 * Send the current sensor temperature
 */
void apiTemperature(float temperature);

/**
 * Send the stream to API server
 */
void apiStream(double x, double y, double z);

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
uint64 getUNIXTimeMS();

#endif
