
#include "common.h"

byte ethernetMac[6] = { 0 };
char deviceid[13] = { 0 };
byte buffer[BUFFER_SIZE];
ushort probeSpeedHz = 100;
uint32_t probeSpeedStat = 0;

#ifdef DEBUG
void printMACAddress() {
  for (int i=0; i < 6; i++) {
    Serial.print(ethernetMac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}
#endif

void setMACAddress(byte* mac) {
  memcpy(ethernetMac, mac, 6);
  snprintf(deviceid, 13, "%02x%02x%02x%02x%02x%02x", ethernetMac[0], ethernetMac[1], ethernetMac[2], ethernetMac[3], ethernetMac[4], ethernetMac[5]);
}
