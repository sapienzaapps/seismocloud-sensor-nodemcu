
#include "common.h"
#include "websocketclient.h"


Client WebSocketClient::*origc = NULL;

int WebSocketClient::connect(IPAddress ip, uint16_t port) {
  // TODO: Connect to HTTP websocket
}

int WebSocketClient::connect(const char *host, uint16_t port) {
  // TODO: Connect to HTTP websocket
}

size_t WebSocketClient::write(uint8_t p) {
  return this->write(&p, 1);
}

size_t WebSocketClient::write(const uint8_t *buf, size_t size) {
  // TODO: Prepare and send a WebSocket packet
  if (size > 125) return -1;
  // TODO: payload > 125

  // Size + headers + mask key
  byte *pkt = (byte*) malloc(size + 2 + 4);
  uint32_t mask = (uint32_t)rand();
  uint8_t len = (uint8_t)size;
  unsigned int i;

  pkt[0] = 0x82;
  pkt[1] = (uint8_t)0x80 | len;
  memcpy(pkt+2, &mask, 4);
  for(i = 0; i < size; i++) {
          pkt[6 + i] = buf[i] ^ pkt[2 + (i % 4)];
  }
  size_t r = this->origc->write(pkt, size + 2 + 4);
  free(pkt);
  return r > 6 ? r - 6 : -1;
}

int WebSocketClient::available() {
  return this->origc->available();
}

int WebSocketClient::read() {
  if (this->origc == NULL) {
    return -1;
  }
  while (this->bufidx >= this->buflen) {
    // Read packet
    uint16_t control;
    byte r;
    int i;
    
    r = this->origc->read((uint8_t*)&control, 2);
    if (r <= 0) {
      return -1;
    }

    bool fin = (control & 0x8000) > 0;
    int opcode = (control & 0x0F00) >> 8;
    bool hasmask = (control & 0x0080) > 0;
    uint16_t len = htons((uint16_t)(control & 0x007F));
    
    if (len == 126 || len == 127) {
      // TODO: handle bigger packets
      return -1;
    }

    if (hasmask) {
      r = this->origc->read(mask, 4);
      if (r != 4) {
        return -1;
      }
    }
    
    this->buflen = min(len, 125);
    r = this->origc->read(this->buf, this->buflen);
    if (r < 0) {
      return -1;
    }
    if (r < buflen) {
      this->buflen = r;
    }

    for (i = 0; i < r && hasmask; i++) {
      buf[i] = buf[i] ^ mask[i % 4];
    }

    switch (opcode) {
      case 0:
        // Continuation (not supported)
        break;
      case 1: // Text
      case 2: // Binary
        bufidx = 0;
        break;
      case 10: // PONG
        break;
      case 9:
        // PING
        // TODO: send a PONG
        break;
      case 8: // Connection close
        this->origc->stop();
      default:
        return -1;
    }
  }
  return this->buf[this->bufidx++];
}

int WebSocketClient::read(uint8_t *buf, size_t size) {
  int i, r;
  for (i = 0; i < size && r >= 0; i++) {
    r = this->read();
    if (r >= 0) {
      buf[i] = r;
    }
  }
  return i;
}

int WebSocketClient::peek() {
  return -1;
}

void WebSocketClient::flush() {
  // Nothing to do
}

void WebSocketClient::stop() {
  this->origc->stop();
}

uint8_t WebSocketClient::connected() {
  return this->origc->connected();
}

WebSocketClient::operator bool() {
  return this->origc->connected();
}

void WebSocketClient::setClient(Client *c) {
  this->origc = c;
}

