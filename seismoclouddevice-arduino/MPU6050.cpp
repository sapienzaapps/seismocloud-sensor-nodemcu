
#include "MPU6050.h"
#ifdef IS_ESP

int16_t IX, IY, IZ, AcX, AcY, AcZ, Tmp;
double iX, iY, iZ, X, Y, Z;
double AccelerationFactor = 0.20 / 32768.0; // Assuming +/- 16G.

AcceleroMPU6050::AcceleroMPU6050() {
}

void AcceleroMPU6050::begin() {
  Wire.begin(4, 5); // sda, scl  // GPIO4 and GPIO5 - on Arduino: Wire.begin()
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(2000);
}

void AcceleroMPU6050::calibrate() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

  IX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  IY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  IZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  iX = IX * AccelerationFactor;
  iY = IY * AccelerationFactor;
  iZ = IZ * AccelerationFactor;
}

int AcceleroMPU6050::getTotalVector() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU_ADDRESS, (size_t)8, true); // request a total of 8 registers

  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)

  X = AcX * AccelerationFactor;
  Y = AcY * AccelerationFactor;
  Z = AcZ * AccelerationFactor;

  // Calculate force
  return sqrt(sq(X - iX) + sq(Y - iY) + sq(Z - iZ));
}

#endif

