
#include "common.h"

AcceleroMMA7361 accelero;
double partialAvg = 0;
double partialStdDev = 0;
unsigned int elements = 0;
double quakeThreshold = 1;
double sigmaIter = 3.0;

void addValueToAvgVar(double val);

void seismometerInit() {
  // Start with standard PIN assignments
  accelero.begin(6, 7, 8, 9, A0, A1, A2);

  accelero.setSensitivity(HIGH);

  accelero.calibrate();

  accelero.setAveraging(10);

  Serial.println();
}

void seismometerTick() {

  int accelVector = accelero.getTotalVector();
  
  if(accelVector > quakeThreshold) {
    LED::red(true);
    // QUAKE
    Serial.print(F("QUAKE: "));
    Serial.print(accelVector);
    Serial.print(F(" > "));
    Serial.println(quakeThreshold);
    httpQuakeRequest();
    delay(5000);
    Serial.println(F("QUAKE Timeout END"));
    LED::red(false);
  }
  addValueToAvgVar(accelVector);
}

double getQuakeThreshold() {
  return quakeThreshold;
}

double getCurrentAVG() {
  return partialAvg;
}

double getCurrentSTDDEV() {
  return sqrt(partialStdDev / (elements - 1));
}

void setSigmaIter(double i) {
  sigmaIter = i;
}

void addValueToAvgVar(double val) {
  elements++;
  double delta = val - partialAvg;
  partialAvg += delta / elements;
  partialStdDev += delta * (val - partialAvg);
  if (elements > 1) {
    quakeThreshold = partialAvg + (getCurrentSTDDEV() * getSigmaIter());
  }
}

void resetLastPeriod() {
  partialAvg = 0;
  partialStdDev = 0;
  elements = 0;
}

double getSigmaIter() {
  return sigmaIter;
}


