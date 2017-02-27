
#include "common.h"

#ifdef IS_ARDUINO
AcceleroMMA7361 accelero;
#else
AcceleroMPU6050 accelero;
#endif

int partialAvg = 0;
int partialStdDev = 0;
unsigned int elements = 0;
int quakeThreshold = 1;
double sigmaIter = 3.0;

void addValueToAvgVar(int val);

void seismometerInit() {
  accelero.begin();
}

void seismometerTick() {

  int accelVector = accelero.getTotalVector();
  
  if(accelVector > quakeThreshold) {
    LED_red(true);
    // QUAKE
#ifdef DEBUG
    Serial.print(F("QUAKE: "));
    Serial.print(accelVector);
    Serial.print(F(" > "));
    Serial.println(quakeThreshold);
#endif
    apiQuake();
    delay(5000);
#ifdef DEBUG
    Serial.println(F("QUAKE Timeout END"));
#endif
    LED_red(false);
  }
  addValueToAvgVar(accelVector);
}

int getQuakeThreshold() {
  return quakeThreshold;
}

int getCurrentAVG() {
  return partialAvg;
}

int getCurrentSTDDEV() {
  return sqrt(partialStdDev / (elements - 1));
}

void setSigmaIter(double i) {
  sigmaIter = i;
}

void addValueToAvgVar(int val) {
  elements++;
  int delta = val - partialAvg;
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


