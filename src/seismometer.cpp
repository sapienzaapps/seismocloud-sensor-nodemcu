
#include "common.h"
#include "cma.h"

CMA_StdDev partialCma;
CMA_StdDev lastCma;

float sigmaIter = 6.0;

double accelVector;
unsigned long lastQuakeMillis = 0;
unsigned long lastAcceleroTick = 0;
unsigned long lastProbeMs = 0;
unsigned long lastThresholdUpdate = 0;
double quakeThreshold = 0;
uint16_t partialProbeSpeedStat = 0;

void rotateThreshold() {
  CMA_STDDEV_COPY(lastCma, partialCma);
  CMA_STDDEV_RESET(partialCma);
  quakeThreshold = lastCma.value + (CMA_STDDEV_GET(lastCma) * sigmaIter);

  snprintf((char*)buffer, BUFFER_SIZE, "Threshold rotation: %lf", quakeThreshold);
  Debugln((char*)buffer);
}

void seismometerInit() {
  MPU6050_begin();

  // Pre-fill threshold
  unsigned long oneSecondMs = millis();
  while(millis()-oneSecondMs < 1000) {
    MPU6050_probe();
    accelVector = sqrt(sq(acceleroX) + sq(acceleroY) + sq(acceleroZ));
    CMA_STDDEV_ADD(partialCma, accelVector);
  }

  rotateThreshold();
}

void setNewSigma(float newSigma) {
  sigmaIter = newSigma;
  quakeThreshold = lastCma.value + (CMA_STDDEV_GET(lastCma) * newSigma);
}

void seismometerTick() {
  if (millis()-lastThresholdUpdate > 15*60*1000) {
    rotateThreshold();
    lastThresholdUpdate = millis();
  }

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
  MPU6050_probe();
  accelVector = sqrt(sq(acceleroX) + sq(acceleroY) + sq(acceleroZ));

  // We're inside the 5 seconds quake condition?
  boolean inQuake = (millis()-lastQuakeMillis) < 5000;

  if(!inQuake && accelVector > quakeThreshold) {
    // QUAKE
    LED_quake();

#ifdef DEBUG
    snprintf((char*)buffer, BUFFER_SIZE, "QUAKE: %lf > %lf", accelVector, quakeThreshold);
    Debugln((char*)buffer);
#endif

    apiQuake();
    lastQuakeMillis = millis();
  } else if (!inQuake) {
    // End quake period
    LED_quake_end();
  }

  // Stream data to server if streaming is enabled
  if (streamingEnabled) {
    apiStream();
  }
  
  // Add value to threshold calculator
  CMA_STDDEV_ADD(partialCma, accelVector);
}
