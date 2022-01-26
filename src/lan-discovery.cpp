#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include "lan-discovery.h"

// LAN discovery
WiFiUDP cmdsock;

void commandInterfaceInit() {
  Debugln("Command interface init");
  cmdsock.begin(62001);

  if(MDNS.begin(hostname)) {
    Debugln("mDNS responder OK");

    MDNS.addService("scs", "tcp", 62001);
    MDNS.addServiceTxt("scs", "tcp", "version", VERSION);
    MDNS.addServiceTxt("scs", "tcp", "model", MODEL);
    MDNS.addServiceTxt("scs", "tcp", "id", deviceid);
  } else {
    Debugln("Can't start the mDNS responder");
  }
}

void commandInterfaceTick() {
  MDNS.update();

  cmdsock.parsePacket();
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

    memcpy(buffer + 12, VERSION, strlen(VERSION) < 4 ? strlen(VERSION) : 4);
    memcpy(buffer + 16, MODEL, strlen(MODEL) < 4 ? strlen(MODEL) : 8);

    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(buffer, 24);
    cmdsock.endPacket();
    cmdsock.flush();
    Debugln("Reply packet sent");
  }
}
