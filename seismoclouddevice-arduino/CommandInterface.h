
#define CMD_INTERFACE_PORT 62001
#define PACKET_SIZE 252

byte udpPacketBuffer[PACKET_SIZE];

typedef enum {
  PKTTYPE_DISCOVERY = 1,
  PKTTYPE_DISCOVERY_REPLY = 2,
  PKYTYPE_PING = 3,
  PKYTYPE_PONG = 4,
  PKTTYPE_START = 5,
  PKTTYPE_STOP = 6,
  PKTTYPE_SENDGPS = 7,
  PKTTYPE_OK = 8,
  PKTTYPE_SETSYSLOG = 9,
  PKTTYPE_REBOOT = 10,
  PKTTYPE_GETINFO = 11,
  PKTTYPE_GETINFO_REPLY = 12
} PacketType;

EthernetUDP cmdsock;

void commandInterfaceInit() {
  cmdsock.begin(62001);
}

void commandInterfaceTick() {
  int packetSize = cmdsock.parsePacket();
  if(cmdsock.available()) {
    Serial.print(F("Received packet of size "));
    Serial.println(packetSize);
    Serial.print(F("From "));
    IPAddress remote = cmdsock.remoteIP();
    for (int i =0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(F(", port "));
    Serial.println(cmdsock.remotePort());

    // read the packet into packetBufffer
    cmdsock.read(udpPacketBuffer, PACKET_SIZE);

    if(udpPacketBuffer[0] != 'I' || udpPacketBuffer[1] != 'N' || udpPacketBuffer[2] != 'G' || udpPacketBuffer[3] != 'V' && udpPacketBuffer[4] != 0) {
      return;
    }

    bool reboot = false;
    unsigned long unixTimeM = getUNIXTime();

    switch(udpPacketBuffer[5]) {
      case PKTTYPE_DISCOVERY:
        // Reply to discovery
        udpPacketBuffer[5] = PKTTYPE_DISCOVERY_REPLY;

        // Copy MAC Address
        memcpy(udpPacketBuffer + 6, mac, 6);
        memcpy(udpPacketBuffer + 12, "1.00", 4);
        memcpy(udpPacketBuffer + 16, "uno", 3);
        break;
      case PKYTYPE_PING:
        // Reply to ping
        udpPacketBuffer[5] = PKYTYPE_PONG;
        break;
      case PKTTYPE_SENDGPS:
        // Get coords
        udpPacketBuffer[5] = PKTTYPE_OK;

        memcpy(&latitude, udpPacketBuffer + 12, 4);
        memcpy(&longitude, udpPacketBuffer + 16, 4);
        
        break;
      case PKTTYPE_REBOOT:
        // Reboot
        // Reply with OK
        udpPacketBuffer[5] = PKTTYPE_OK;
        reboot = true;
        break;
      case PKTTYPE_GETINFO:
        udpPacketBuffer[5] = PKTTYPE_GETINFO_REPLY;

        memcpy(udpPacketBuffer + 6, mac, 6);
        memcpy(udpPacketBuffer + 28, &bootTime, 4);
        memcpy(udpPacketBuffer + 32, &unixTimeM, 4);
        memcpy(udpPacketBuffer + 36, "1.00", 4);
        memcpy(udpPacketBuffer + 53, "uno", 3);
        memcpy(udpPacketBuffer + 57, "MMA7361", 7);

        break;
      default:
        // Unknown packet or invalid command
        return;
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

