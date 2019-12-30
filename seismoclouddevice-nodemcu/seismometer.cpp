
#include "common.h"

AcceleroMPU6050 accelero;

double partialAvg = 0;
double partialStdDev = 0;
unsigned int elements = 0;
double quakeThreshold = 1;
double sigmaIter = 3.0;

double accelVector, x, y, z;
unsigned long lastQuakeMillis = 0;
unsigned long lastAcceleroTick = 0;

unsigned long lastProbeMs = 0;
uint32_t partialProbeSpeedStat = 0;

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
    quakeThreshold = partialAvg + (getCurrentSTDDEV() * sigmaIter);
  }
}

void seismometerInit() {
  accelero.begin();
}

void seismometerTick() {
  // Probe speed limiter
  if (probeSpeedHz > 0 && millis()-lastAcceleroTick < (1000/probeSpeedHz)) {
    return;
  }
  lastAcceleroTick = millis();

  // Probe speed statistics
  if(millis() - lastProbeMs >= 1000) {
    lastProbeMs = millis();
    probeSpeedStat = partialProbeSpeedStat;
    
    Debug(F("Probe speed (values per second): "));
    Debugln(probeSpeedStat);

    partialProbeSpeedStat = 0;
  }
  partialProbeSpeedStat++;

  // Read values
  accelVector = accelero.getTotalVector(&x, &y, &z);

  // We're inside the 5 seconds quake condition?
  boolean inQuake = (millis()-lastQuakeMillis) < 5000;

  if(!inQuake && accelVector > quakeThreshold) {
    // QUAKE
    LED_red(true);

#ifdef DEBUG
    memset(buffer, 0, BUFFER_SIZE);
    snprintf((char*)buffer, BUFFER_SIZE, "QUAKE: %lf > %lf", accelVector, quakeThreshold);
    Debugln((char*)buffer);
#endif

    apiQuake(x, y, z);
    lastQuakeMillis = millis();
  } else if (!inQuake) {
    // End quake period
    LED_red(false);
  }

  // Stream data to server if streaming is enabled
  if (streamingEnabled) {
    apiStream(x, y, z);
  }
  
  // Add value to threshold calculator
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

double getCurrentSTDDEV() {
  return sqrt(partialStdDev / (elements - 1));
}
