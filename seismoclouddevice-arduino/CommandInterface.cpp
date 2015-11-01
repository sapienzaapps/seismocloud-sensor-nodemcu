
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

    if(udpPacketBuffer[0] != 'S' || udpPacketBuffer[1] != 'E' || udpPacketBuffer[2] != 'I' || udpPacketBuffer[3] != 'S' && udpPacketBuffer[4] != 'M' && udpPacketBuffer[5] != 'O') {
      Serial.println(F("Invalid magic, dropping packet"));
      return;
    }
    if(udpPacketBuffer[6] != PROTOCOL_VERSION) {
      Serial.println(F("Invalid protocol version, dropping packet"));
      return;
    }

    bool reboot = false;
    unsigned long unixTimeM = getUNIXTime();
    unsigned long uptime = getUNIXTime() - getBootTime();
    byte* uuidNumber = getUuidNumber();
    uint32_t probeSpeed = getProbeSpeedStatistic();

    float longitude = 0;
    float latitude = 0;

    switch(udpPacketBuffer[7]) {
      case PKTTYPE_DISCOVERY:
        // Reply to discovery
        udpPacketBuffer[7] = PKTTYPE_DISCOVERY_REPLY;

        // Copy uuidNumber
        memcpy(udpPacketBuffer + 8, uuidNumber, 16);
        
        memcpy(udpPacketBuffer + 24, "1.00", 4);
        memcpy(udpPacketBuffer + 28, "uno", 3);
        break;
      case PKYTYPE_PING:
        // Reply to ping
        udpPacketBuffer[7] = PKYTYPE_PONG;
        break;
      case PKTTYPE_SENDGPS:
        // Get coords
        udpPacketBuffer[7] = PKTTYPE_OK;

        memcpy(&latitude, udpPacketBuffer + 24, 4);
        memcpy(&longitude, udpPacketBuffer + 28, 4);
        
        break;
      case PKTTYPE_REBOOT:
        // Reboot
        // Reply with OK
        udpPacketBuffer[7] = PKTTYPE_OK;
        reboot = true;
        break;
      case PKTTYPE_GETINFO:
        udpPacketBuffer[7] = PKTTYPE_GETINFO_REPLY;

        memcpy(udpPacketBuffer + 8, uuidNumber, 16);
        memcpy(udpPacketBuffer + 40, &uptime, 4);
        memcpy(udpPacketBuffer + 44, &unixTimeM, 4);
        memcpy(udpPacketBuffer + 48, "1.00", 4);
        memcpy(udpPacketBuffer + 65, "uno", 3);
        memcpy(udpPacketBuffer + 69, "MMA7361", 7);
        memcpy(udpPacketBuffer + 77, &probeSpeed, 4);

        break;
      default:
        // Unknown packet or invalid command
        return;
    }

    if(longitude != 0 && latitude != 0) {
      setLongitude(longitude);
      setLatitude(latitude);
    }

    cmdsock.beginPacket(cmdsock.remoteIP(), cmdsock.remotePort());
    cmdsock.write(udpPacketBuffer, PACKET_SIZE);
    cmdsock.endPacket();
    cmdsock.flush();

    if(reboot) {
      soft_restart();
    }
  }
}

