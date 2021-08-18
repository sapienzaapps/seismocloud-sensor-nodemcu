
#ifndef __API_H
#define __API_H

/**
 * Connect to API server
 */
bool apiConnect();

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

void parseConfig(const char *payload);
extern String rpiHostName;
extern uint16_t rpiPort;

#endif
