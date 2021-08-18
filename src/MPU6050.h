#ifndef __MPU6050_H
#define __MPU6050_H

#define MPU_ADDRESS   0x68

#ifndef WIRE_SDA
#define WIRE_SDA      D1
#endif

#ifndef WIRE_SCL
#define WIRE_SCL      D2
#endif

#ifdef DEBUG
#define CALIBRATION_SECONDS  3
#else
#define CALIBRATION_SECONDS  10
#endif

bool MPU6050_begin();
void MPU6050_probe();

extern float Tmp;
extern double acceleroX, acceleroY, acceleroZ;

#endif
