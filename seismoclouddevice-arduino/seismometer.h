
#ifndef __SEISMOMETER_H
#define __SEISMOMETER_H

void seismometerInit();
void seismometerTick();
void firstTimeThresholdCalculation();
void setSigmaIter(double i);
double getSigmaIter();
void resetLastPeriod();

#endif
