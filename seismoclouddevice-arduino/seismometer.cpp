
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

  if(accelVector > 0.3) {
    // QUAKE
    //Serial.println("QUAKE");
    //httpQuakeRequest();
    //delay(1000 * 5);
  }
}

