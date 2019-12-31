
#include "common.h"
#include "cma.h"

CMA_StdDev partialCma;

double quakeThreshold = 1;
float sigmaIter = 3.0;

double accelVector;
unsigned long lastQuakeMillis = 0;
unsigned long lastAcceleroTick = 0;
unsigned long lastProbeMs = 0;

uint16_t partialProbeSpeedStat = 0;

/**
 * Add a new value to mean and standard deviation values
 * Knuth mean/avg calculation algorithm
 */
void addValueToAvgVar(double val) {
  CMA_STDDEV_ADD(partialCma, val);
  if (partialCma.count > 1) {
    quakeThreshold = partialCma.value + (CMA_STDDEV_GET(partialCma) * sigmaIter);
  }
}

void seismometerInit() {
  MPU6050_begin();
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
  MPU6050_probe();
  accelVector = sqrt(sq(acceleroX) + sq(acceleroY) + sq(acceleroZ));

  // We're inside the 5 seconds quake condition?
  boolean inQuake = (millis()-lastQuakeMillis) < 5000;

  if(!inQuake && accelVector > quakeThreshold) {
    // QUAKE
    LED_quake();

#ifdef DEBUG
    memset(buffer, 0, BUFFER_SIZE);
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
  addValueToAvgVar(accelVector);
}

void resetLastPeriod() {
  CMA_STDDEV_RESET(partialCma);
}
