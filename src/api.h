
#ifndef __API_H
#define __API_H

#include "common.h"

/**
 * Connect to API server
 */
boolean apiConnect();

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

#endif
