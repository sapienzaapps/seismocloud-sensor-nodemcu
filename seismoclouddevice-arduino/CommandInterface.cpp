
#include <Arduino.h>
#include "common.h"

byte udpPacketBuffer[PACKET_SIZE];
EthernetUDP cmdsock;

void commandInterfaceInit() {
  cmdsock.begin(62001);
}

void commandInterfaceTick() {
  int packetSize = cmdsock.parsePacket();
  if(cmdsock.available()) {
    
    // read the packet into packetBufffer
    cmdsock.read(udpPacketBuffer, PACKET_SIZE);

    if(memcmp("INGV\0", udpPacketBuffer, 5) != 0 || udpPacketBuffer[5] != PKTTYPE_DISCOVERY) {
      return;
    }

    byte macaddress[6] = { 0 };
    getMACAddress(macaddress);

    // Reply to discovery
    udpPacketBuffer[5] = PKTTYPE_DISCOVERY_REPLY;

    memcpy(udpPacketBuffer + 6, macaddress, 6);
    
    memcpy(udpPacketBuffer + 12, VERSION, min(strlen(VERSION), 4));
    memcpy(udpPacketBuffer + 16, "uno", 3);
    
    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(udpPacketBuffer, PACKET_SIZE);
    cmdsock.endPacket();
    cmdsock.flush();
  }
}

