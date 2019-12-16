
#include "common.h"

#ifdef IS_ESP
AcceleroMPU6050 accelero;
#endif

double partialAvg = 0;
double partialStdDev = 0;
unsigned int elements = 0;
double quakeThreshold = 1;
double sigmaIter = 3.0;

double accelVector, x, y, z;

/**
 * Returns the current quake threshold
 */
double getQuakeThreshold() {
  return quakeThreshold;
}

/**
 * Get the current mean (average)
 */
double getCurrentAVG() {
  return partialAvg;
}

/**
 * Add a new value to mean and standard deviation values
 * Knuth mean/avg calculation algorithm
 */
void addValueToAvgVar(double val) {
  elements++;
  double delta = val - partialAvg;
  partialAvg += delta / elements;
  partialStdDev += delta * (val - partialAvg);
  if (elements > 1) {
    quakeThreshold = partialAvg + (getCurrentSTDDEV() * getSigmaIter());
  }
}

void seismometerInit() {
  accelero.begin();
}

void seismometerTick() {

  accelVector = accelero.getTotalVector(&x, &y, &z);

  if(accelVector > quakeThreshold) {
    LED_red(true);
    // QUAKE
    Debug(F("QUAKE: "));
    Debug(accelVector);
    Debug(F(" > "));
    Debugln(quakeThreshold);
    apiQuake(x, y, z);
    delay(5000);
    Debugln(F("QUAKE Timeout END"));
    LED_red(false);
  }

  if (streamingEnabled) {
    apiStream(x, y, z);
  }
  
  addValueToAvgVar(accelVector);
}

void setSigmaIter(double i) {
  sigmaIter = i;
}

void resetLastPeriod() {
  partialAvg = 0;
  partialStdDev = 0;
  elements = 0;
}

double getSigmaIter() {
  return sigmaIter;
}

double getCurrentSTDDEV() {
  return sqrt(partialStdDev / (elements - 1));
}
