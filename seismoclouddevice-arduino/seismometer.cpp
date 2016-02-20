
#include "common.h"

AcceleroMMA7361 accelero;
double lastPeriodAvg = 0;
double lastPeriodSQM = 0;
unsigned int elements = 0;
double quakeThreshold = 0.3;
double sigmaIter = 1;

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

  addValueToAvgVar(accelVector);

  if(accelVector > quakeThreshold) {
    LED::red(true);
    // QUAKE
    Serial.print(F("QUAKE: "));
    Serial.println(accelVector);
    httpQuakeRequest();
    delay(5000);
    Serial.println(F("QUAKE Timeout END"));
    LED::red(false);
  }
}

double getQuakeThreshold() {
  return quakeThreshold;
}

double getLastPeriodAVG() {
  return lastPeriodAvg;
}

double getLastPeriodVAR() {
  return lastPeriodSQM / elements;
}

// TODO: read from alive.php
void setSigmaIter(double i) {
  double newquakeThreshold = getLastPeriodAVG() + (getLastPeriodVAR() * i);
  if(newquakeThreshold != NAN) {
    Serial.print(F("Attempting to set quake threshold to "));
    Serial.print(newquakeThreshold);
    Serial.print(F(" (old "));
    Serial.print(quakeThreshold);
    Serial.print(F(") iter "));
    Serial.print(i);
    Serial.print(F(" elem "));
    Serial.println(elements);
    sigmaIter = i;
    quakeThreshold = newquakeThreshold;
  }
}

void addValueToAvgVar(double val) {
  double precAvg = lastPeriodAvg;

  elements++;
  lastPeriodAvg = precAvg + ((val - precAvg)/elements);
  lastPeriodSQM = lastPeriodSQM + ((val - precAvg)*(val - lastPeriodAvg));
}

void resetLastPeriod() {
  lastPeriodAvg = 0;
  lastPeriodSQM = 0;
  elements = 0;
}

double getSigmaIter() {
  return sigmaIter;
}

void firstTimeThresholdCalculation() {
  for(int i=0; i < 200; i++) {
    addValueToAvgVar(accelero.getTotalVector());
  }
  setSigmaIter(getSigmaIter());
  Serial.print(F("First time AVG, VAR and Threshold:"));
  Serial.print(getLastPeriodAVG());
  Serial.print(" ");
  Serial.print(getLastPeriodVAR());
  Serial.print(" ");
  Serial.println(quakeThreshold);
}



