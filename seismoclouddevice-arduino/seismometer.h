
AcceleroMMA7361 accelero;

void seismometerInit() {
  // Start with standard PIN assignments
  accelero.begin();

  accelero.setSensitivity(HIGH);

  accelero.calibrate();
}

void seismometerTick() {

  int accelVector = accelero.getTotalVector();

  if(accelVector > 0.3) {
    // QUAKE
    httpQuakeRequest();
    delay(1000 * 5);
  }
}

