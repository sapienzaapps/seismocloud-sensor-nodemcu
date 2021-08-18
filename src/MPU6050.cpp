
#include <Arduino.h>
#include <Wire.h>
#include "MPU6050.h"
#include "cma.h"
#include "debug_print.h"

constexpr double AccelerationFactor (0.20/32768.0); // Assuming +/- 16G.

double calibrationX, calibrationY, calibrationZ, acceleroX, acceleroY, acceleroZ;
float Tmp;

bool MPU6050_begin() {
  Wire.begin(WIRE_SDA, WIRE_SCL); // sda, scl  // GPIO4 and GPIO5 - on Arduino: Wire.begin()
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(2000);

  int16_t AcX, AcY, AcZ, AcTmp = 0;
  CMA AvgX, AvgY, AvgZ;
  unsigned long startms = millis();
  CMA_RESET(AvgX); CMA_RESET(AvgY); CMA_RESET(AvgZ);

  while(millis()-startms < CALIBRATION_SECONDS*1000) {
    Wire.beginTransmission(MPU_ADDRESS);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    AcTmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

    // Calculate average values
    CMA_ADD(AvgX, AcX);
    CMA_ADD(AvgY, AcY);
    CMA_ADD(AvgZ, AcZ);

    delay(10);
  }

  // Error during calibration
  if (AvgX.value == -1 && AvgY.value == -1 && AvgZ.value == -1) {
    Debugln(F("[MPU6050] Calibration error - error reading values"));
    return false;
  }

  calibrationX = AvgX.value * AccelerationFactor;
  calibrationY = AvgY.value * AccelerationFactor;
  calibrationZ = AvgZ.value * AccelerationFactor;

  Tmp = AcTmp / 340.00 + 36.53;
  
#ifdef DEBUG
  char buffer[100] = { 0 };
  snprintf(buffer, 100, "[MPU6050] Calibration X:%f Y:%f Z:%f Temp:%f", calibrationX, calibrationY, calibrationZ, Tmp);
  Debugln(buffer);
#endif
  return true;
}

void MPU6050_probe() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

  acceleroX = ((int16_t)(Wire.read() << 8 | Wire.read()) * AccelerationFactor) - calibrationX;
  acceleroY = ((int16_t)(Wire.read() << 8 | Wire.read()) * AccelerationFactor) - calibrationY;
  acceleroZ = ((int16_t)(Wire.read() << 8 | Wire.read()) * AccelerationFactor) - calibrationZ;

  Tmp = (float)((int16_t)(Wire.read() << 8 | Wire.read())) / 340.00 + 36.53;
}
