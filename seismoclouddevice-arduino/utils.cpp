
#include "common.h"

String macToString(const byte* mac) {
  char buf[13];
  memset(buf, 0, 13);
  snprintf(buf, 13, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void reverse4bytes(byte* memory) {
  byte val[4];
  memcpy(&val, memory, 4);
  memcpy(memory+0, &val[3], 1);
  memcpy(memory+1, &val[2], 1);
  memcpy(memory+2, &val[1], 1);
  memcpy(memory+3, &val[0], 1);
}

unsigned long tcpLatency() {
  unsigned long starts = millis();
  EthernetClient client;
  // if there's a successful connection:
  int cresult = client.connect(DEFAULTHOST, 80);
  client.stop();
  return millis() - starts;
}

