#ifndef __WEBSOCKETCLIENT_H
#define __WEBSOCKETCLIENT_H

#include <Client.h>

class WebSocketClient : public Client {
public:
  int connect(IPAddress ip, uint16_t port);
  int connect(const char *host, uint16_t port);
  size_t write(uint8_t);
  size_t write(const uint8_t *buf, size_t size);
  int available();
  int read();
  int read(uint8_t *buf, size_t size);
  int peek();
  void flush();
  void stop();
  uint8_t connected();
  operator bool();

  void setClient(Client *c);
private:
  Client *origc;
  byte mask[4];
  byte buf[127];
  byte buflen = 0;
  byte bufidx = 0;
};

#endif
