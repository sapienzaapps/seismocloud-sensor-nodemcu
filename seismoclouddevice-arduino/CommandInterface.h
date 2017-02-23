
#ifndef __COMMANDINTERFACE_H
#define __COMMANDINTERFACE_H

#define CMD_INTERFACE_PORT  62001
#define PACKET_SIZE         50

typedef enum {
  PKTTYPE_DISCOVERY = 1,
  PKTTYPE_DISCOVERY_REPLY = 2
} PacketType;

void commandInterfaceInit();

void commandInterfaceTick();

#endif
