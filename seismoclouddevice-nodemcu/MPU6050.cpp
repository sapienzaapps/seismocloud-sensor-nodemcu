
#include "MPU6050.h"

int16_t AcX, AcY, AcZ, AcTmp;
double iX, iY, iZ, X, Y, Z;
float Tmp;
#define AccelerationFactor (0.20/32768.0) // Assuming +/- 16G.

void AcceleroMPU6050::begin() {
  Wire.begin(WIRE_SDA, WIRE_SCL); // sda, scl  // GPIO4 and GPIO5 - on Arduino: Wire.begin()
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(2000);
  calibrate();
}

void AcceleroMPU6050::calibrate() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  AcTmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  Tmp = AcTmp / 340.00 + 36.53;

  iX = AcX * AccelerationFactor;
  iY = AcY * AccelerationFactor;
  iZ = AcZ * AccelerationFactor;

  Debugln("Calibrated with");
  Debug("X:");
  Debugln(iX);
  Debug("Y:");
  Debugln(iY);
  Debug("Z:");
  Debugln(iZ);
  Debug("Temp:");
  Debugln(Tmp);
  Debug("Accel factor: ");
  Debugln(AccelerationFactor);
}

double AcceleroMPU6050::getTotalVector(double *x, double *y, double *z) {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  AcTmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  Tmp = AcTmp / 340.00 + 36.53;

  X = AcX * AccelerationFactor;
  Y = AcY * AccelerationFactor;
  Z = AcZ * AccelerationFactor;

  if (x != NULL) {
    *x = X - iX;
  }
  if (y != NULL) {
    *y = Y - iY;
  }
  if (z != NULL) {
    *z = Z - iZ;
  }

  // Calculate force
  return sqrt(sq(X - iX) + sq(Y - iY) + sq(Z - iZ));
}
