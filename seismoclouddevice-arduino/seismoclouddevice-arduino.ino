
#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0, 0, 0, 0, 0, 0
};

EthernetClient client;
float latitude = 0;
float longitude = 0;
unsigned long bootTime = 0;

#include "SoftReset.h"
#include "AcceleroMMA7361.h"
#include "ntp.h"
#include "utils.h"
#include "httpclient.h"
#include "CommandInterface.h"
#include "seismometer.h"

unsigned long lastAliveMs = 0;
unsigned long lastSeismoMs = 0;

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println(F("Booting SeismoCloudDevice-Arduino sketch"));

  byte zeromac[6];
  memset(zeromac, 0, 6);
  if(memcmp(zeromac, mac, 6) == 0) {
    Serial.println(F("No MAC Address configured - please edit sketch file"));
    while(true);
  }

  // give the ethernet module time to boot up:
  delay(1000);
  
  Ethernet.begin(mac);
  
  // print the Ethernet board/shield's IP address:
  Serial.print(F("My IP address: "));
  Serial.println(Ethernet.localIP());

  Serial.println(F("Updating NTP Time"));
  do {
    updateNTP();
    bootTime = getUNIXTime();
    if(bootTime == 0) {
      Serial.println(F("NTP update failed, retrying in 5 seconds..."));
      delay(5 * 1000);
    }
  } while(bootTime == 0);

  Serial.print(F("Local time is"));
  Serial.println(bootTime);

  Serial.println(F("Init command interface"));
  commandInterfaceInit();

  if(latitude == 0 && longitude == 0) {
    Serial.println(F("No position available, waiting for GPS from phone App"));
    do {
      commandInterfaceTick();
    } while(latitude == 0 && longitude == 0);
    Serial.print(F("New position: lat:"));
    Serial.print(latitude);
    Serial.print(F(" lon:"));
    Serial.println(longitude);
  }

  Serial.println(F("Init seismometer and calibrate"));
  seismometerInit();

  Serial.println(F("Send keep-alive to server..."));
  httpAliveRequest();
  lastAliveMs = millis();
  Serial.println(F("Keep-alive sent, boot completed"));
}

void loop() {

  // Update NTP (if necessary)
  updateNTP();

  commandInterfaceTick();

  // Calling alive every 14 minutes
  if(millis() - lastAliveMs >= 1000 * 60 * 14) {
    httpAliveRequest();
    lastAliveMs = millis();
  }

  // Detection
  // TODO: check if we need to delay measurements by 60 seconds
  if(millis() - lastSeismoMs >= 60) {
    seismometerTick();
    lastSeismoMs = millis();
  }
}
