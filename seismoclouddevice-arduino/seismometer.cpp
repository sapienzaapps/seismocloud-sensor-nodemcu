
#include "common.h"

AcceleroMMA7361 accelero;

void seismometerInit() {
  // Start with standard PIN assignments
  accelero.begin(6, 7, 8, 9, A0, A1, A2);

  accelero.setSensitivity(HIGH);

  accelero.calibrate();

  accelero.setAveraging(10);
}

void seismometerTick() {

  int accelVector = accelero.getTotalVector();

  // TODO: better checking
  if(accelVector > 250) {
    // QUAKE
    Serial.print("QUAKE: ");
    Serial.println(accelVector);
    httpQuakeRequest();
    delay(1000 * 5);
    Serial.println("QUAKE Timeout END");
  }
}

