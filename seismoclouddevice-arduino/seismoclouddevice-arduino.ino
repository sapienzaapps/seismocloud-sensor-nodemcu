
#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0, 0, 0, 0, 0, 0
};

EthernetClient client;

#include "AcceleroMMA7361.h"

#include "utils.h"
#include "httpclient.h"

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  byte zeromac[6];
  if(memcmp(zeromac, mac, 6) == 0) {
    Serial.println("No MAC Address configured - please edit sketch file");
  }

  // give the ethernet module time to boot up:
  delay(1000);
  
  Ethernet.begin(mac);
  
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

unsigned long lastAliveMs = 0;

void loop() {
  if(millis() - lastAliveMs >= 1000 * 14) {
    httpAliveRequest();
    lastAliveMs = millis();
  }
}
