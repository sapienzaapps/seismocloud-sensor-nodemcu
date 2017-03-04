
#include "update.h"

#ifndef DONT_UPDATE

#ifdef IS_ESP
// https://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html
bool update(char *host, char *path) {
  // Checksum should be into "x-MD5" HTTP header
  Debug("Requesting update from ");
  Debug(host);
  Debug(" path: ");
  Debugln(path);
  HTTPUpdateResult r = ESPhttpUpdate.update(host, 80, path);
  return r == HTTPUpdateResult::HTTP_UPDATE_OK;
}
#endif


#ifdef IS_ARDUINO
FATFS fs;
EthernetClient updateClient;
MyRingBuffer buf(12);

bool update(char *host, char *path) {
  bool ret = false;

  selectSD();
  switch(pf_mount(&fs)) {
    case FR_NOT_READY:
      Debugln(F("Storage error"));
      selectEthernet();
      return ret;
    case FR_DISK_ERR:
      Debug(F("Disk error"));
      selectEthernet();
      return ret;
    case FR_NO_FILESYSTEM:
      Debug(F("no FS"));
      selectEthernet();
      return ret;
  }
  selectEthernet();
  if (updateClient.connect(host, 80) == 1) {
    updateClient.print(F("GET "));
    updateClient.print(path);
    updateClient.println(F(" HTTP/1.0"));
    updateClient.print(F("Host: "));
    updateClient.println(host);
    updateClient.println(F("Connection: close\r\n"));

    while (updateClient.connected() && !updateClient.available()) delay(1);
    bool getOk = false;
    while (updateClient.available()) {
      buf.push(updateClient.read());
      if (!getOk && buf.endsWith("HTTP/1.0 200")) {
        getOk = true;
      }
      if (buf.endsWith("\r\n\r\n")) {
        Debugln("File downloaded");
        switch(pf_open("FIRMWARE.BIN")) {
          case FR_NO_FILE:
            Debugln(F("No file"));
            selectEthernet();
            return ret;
          case FR_DISK_ERR:
            Debug(F("Disk error"));
            selectEthernet();
            return ret;
          case FR_NOT_ENABLED:
            Debug(F("no FS"));
            selectEthernet();
            return ret;
        }
        Debugln("File opened");
        unsigned int s = 0;
        unsigned int r = 0;
        unsigned int totalread = 0;
        switch(pf_lseek(0)) {
          case FR_DISK_ERR:
            Debug(F("Disk error"));
            selectEthernet();
            return ret;
          case FR_NOT_OPENED:
            Debug(F("File not opened"));
            selectEthernet();
            return ret;
        }
        while (updateClient.connected()) {
          int p = updateClient.read();
          if (p == -1) {
            continue;
          }
          buffer[s] = (byte)p;
          s++;
          totalread++;
          if (s == 512) {
            selectSD();
            switch(pf_write(buffer, 512, &r)) {
              case FR_NOT_ENABLED:
                Debug(F("Disk not enabled"));
                selectEthernet();
                return ret;
              case FR_DISK_ERR:
                Debug(F("Disk error"));
                selectEthernet();
                return ret;
              case FR_NOT_OPENED:
                Debug(F("File not opened"));
                selectEthernet();
                return ret;
            }
            Debug(F("Written bytes: "));
            Debugln(r);
            s = 0;
            selectEthernet();
          }
        }
        selectSD();
        if (s > 0) {
          switch(pf_write(buffer, s, &r)) {
            case FR_NOT_ENABLED:
              Debug(F("Disk not enabled during write"));
              selectEthernet();
              return ret;
            case FR_DISK_ERR:
              Debug(F("Disk error during write"));
              selectEthernet();
              return ret;
            case FR_NOT_OPENED:
              Debug(F("File not opened during write"));
              selectEthernet();
              return ret;
          }
          Debug(F("Written bytes: "));
          Debugln(r);
        }
        pf_write(0, 0, &r);
        Debug(F("Total received: "));
        Debugln(totalread);
        Debugln(F("Update done"));
        soft_restart();
        ret = true;
      }
    }
    updateClient.stop();
  }
  selectEthernet();
  return ret;
}

#endif
#endif
