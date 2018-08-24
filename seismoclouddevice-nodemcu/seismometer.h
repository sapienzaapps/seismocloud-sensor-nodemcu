
#ifndef __SEISMOMETER_H
#define __SEISMOMETER_H

/**
 * Initial power-on sequence and calibration (if needed)
 */
void seismometerInit();

/**
 * Check seismometer acceleration and see if there is a over-threshold aceleration
 * If the value is over threshold, an API event ("quake") is raised and then
 * the accelerometer sleeps for 5 seconds
 */
void seismometerTick();

/**
 * Set the sigma multiplier for threshold
 */
void setSigmaIter(double i);

/**
 * Returns current sigma multiplier
 */
double getSigmaIter();

/**
 * Reset mean and standard dev values
 */
void resetLastPeriod();

/**
 * Returns the current standard deviation value
 * Knuth variance calculation algorithm
 */
double getCurrentSTDDEV();

#endif
