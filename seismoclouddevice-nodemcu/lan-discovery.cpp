#include "lan-discovery.h"

// LAN discovery
#ifdef IS_ESP
WiFiUDP cmdsock;
#endif

void commandInterfaceInit() {
  Debugln("Command interface init");
  cmdsock.begin(62001);
}

void commandInterfaceTick() {
  int packetSize = cmdsock.parsePacket();
  if (cmdsock.available()) {

    // read the packet into packetBufffer
    cmdsock.read(buffer, BUFFER_SIZE);
    Debugln("New packet received");

    if (memcmp("INGV\0", buffer, 5) != 0 || buffer[5] != PKTTYPE_DISCOVERY) {
      Debugln("Invalid packet");
      return;
    }

    // Reply to discovery
    buffer[5] = PKTTYPE_DISCOVERY_REPLY;

    memcpy(buffer + 6, ethernetMac, 6);

    memcpy(buffer + 12, VERSION, min(strlen(VERSION), 4));
    memcpy(buffer + 16, MODEL, min(strlen(MODEL), 8));

    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(buffer, 24);
    cmdsock.endPacket();
    cmdsock.flush();
    Debugln("Reply packet sent");
  }
}
