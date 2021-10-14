
#include "common.h"
#include "update.h"
#include <ESP8266httpUpdate.h>
#include <WiFiClientSecure.h>

// https://esp8266.github.io/Arduino/versions/2.0.0/doc/ota_updates/ota_updates.html
void checkForUpdates() {
#ifndef DONT_UPDATE
  Debugln(F("[UPDATE] Starting update check process"));
  BearSSL::WiFiClientSecure client;

  client.setKnownKey(&tlspubkey);

  if (!client.connect(UPDATE_SERVER, 443)) {
    Debugln(F("[UPDATE] Connection failed"));
    client.stop();
    return;
  }

  snprintf((char*)buffer, BUFFER_SIZE, "https://%s/firmware/%s/%s/bin", UPDATE_SERVER, MODEL, deviceid);

  HTTPUpdateResult r = ESPhttpUpdate.update(client, (char*)buffer, VERSION);
  switch (r) {
    case HTTPUpdateResult::HTTP_UPDATE_OK:
      Debugln(F("[UPDATE] Update OK, reboot"));
      soft_restart();
      while(true) {};
      break;
    case HTTPUpdateResult::HTTP_UPDATE_NO_UPDATES:
      Debugln(F("[UPDATE] No updates available"));
      break;
    case HTTPUpdateResult::HTTP_UPDATE_FAILED:
    default:
      Debugln(F("[UPDATE] Error checking updates"));
      break;
  }
  client.stop();
#endif
}
