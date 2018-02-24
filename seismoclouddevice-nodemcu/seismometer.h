
#ifndef __SEISMOMETER_H
#define __SEISMOMETER_H

void seismometerInit();
void seismometerTick();
void setSigmaIter(double i);
double getSigmaIter();
void resetLastPeriod();
double getCurrentSTDDEV();

#endif
