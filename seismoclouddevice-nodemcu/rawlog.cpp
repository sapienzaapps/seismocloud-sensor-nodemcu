#include "common.h"
#include "rawlog.h"
#ifdef RAWLOG

void rawlog() {
  Serial.begin(115200);

  double accelVector;
  unsigned long lastCycleMs = 0;
  unsigned long lastProbeMs = 0;
  uint16_t partialProbeSpeedStat = 0;
  
  MPU6050_begin();

  while(true) {
    lastCycleMs = millis();
    // Probe speed statistics
    /*if(millis() - lastProbeMs >= 1000) {
      lastProbeMs = millis();
      probeSpeedStat = partialProbeSpeedStat;
      
      Debug(F("Probe speed (values per second): "));
      Debugln(probeSpeedStat);

      partialProbeSpeedStat = 0;
    }
    partialProbeSpeedStat++;*/

    // Read values
    MPU6050_probe();
    accelVector = sqrt(sq(acceleroX) + sq(acceleroY) + sq(acceleroZ));

    snprintf((char*)buffer, BUFFER_SIZE, "%lf\t%lf\t%lf\t%lf\t%lf", millis()/1000.0,
      acceleroX, acceleroY, acceleroZ, accelVector);
    Debugln((char*)buffer);

    // Wait 20ms minus the time from the last cycle (50Hz freq)
    delay(20 - (millis()-lastCycleMs));
  }
}

#endif
