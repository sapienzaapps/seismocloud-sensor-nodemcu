
#include "common.h"

#ifdef IS_ARDUINO
AcceleroMMA7361 accelero;
#endif
#ifdef IS_ESP
AcceleroMPU6050 accelero;
#endif

double partialAvg = 0;
double partialStdDev = 0;
unsigned int elements = 0;
double quakeThreshold = 1;
double sigmaIter = 3.0;

void addValueToAvgVar(double val);

void seismometerInit() {
  accelero.begin();
}

void seismometerTick() {

  double accelVector = accelero.getTotalVector();

  if(accelVector > quakeThreshold) {
    LED_red(true);
    // QUAKE
    Debug(F("QUAKE: "));
    Debug(accelVector);
    Debug(F(" > "));
    Debugln(quakeThreshold);
    apiQuake();
    delay(5000);
    Debugln(F("QUAKE Timeout END"));
    LED_red(false);
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


