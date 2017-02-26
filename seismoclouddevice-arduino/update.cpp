
#include "update.h"

#ifndef DONT_UPDATE

#ifdef IS_ESP
// https://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html
bool update(char *host, char *path, char *checksum) {
  // Checksum should be into "x-MD5" HTTP header
  HTTPUpdateResult r = ESPhttpUpdate.update(host, 80, path);
  return r == HTTPUpdateResult::HTTP_UPDATE_OK;
}
#endif


#ifdef IS_ARDUINO
EthernetClient updateClient;
MyRingBuffer buf(12);

void selectSD() {
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void selectEthernet() {
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
}

bool update(char *host, char *path, char *checksum) {
  bool ret = false;

  selectSD();
  if (SD.begin(4)) {
    if (updateClient.connect(host, 80) == 1) {
      updateClient.print(F("GET "));
      updateClient.print(path);
      updateClient.println(F(" HTTP/1.1"));
      updateClient.print(F("Host: "));
      updateClient.println(host);
      updateClient.println(F("Connection: close\r\n"));
  
      while (updateClient.connected() && !updateClient.available()) delay(1);
      bool getOk = false;
      while (updateClient.available()) {
        buf.push(updateClient.read());
        if (!getOk && buf.endsWith("HTTP/1.1 200")) {
          getOk = true;
        }
        if (buf.endsWith("\r\n\r\n")) {
          File dest = SD.open(F("FIRMWARE.HEX"), FILE_WRITE);
          while (updateClient.available()) {
            dest.write(updateClient.read());
          }
          dest.close();
  
          // TODO: checksum
          ret = true;
        }
      }
      updateClient.stop();
    }
  }
  selectEthernet();
  return ret;
}

#endif
#endif
