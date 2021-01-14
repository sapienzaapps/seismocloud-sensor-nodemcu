#ifndef __MPU6050_H
#define __MPU6050_H

#include "common.h"

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDRESS   0x68

#ifdef MODEL_esp01
#define WIRE_SDA      2
#define WIRE_SCL      0
#else
#define WIRE_SDA      D1
#define WIRE_SCL      D2
#endif

#ifdef DEBUG
#define CALIBRATION_SECONDS  3
#else
#define CALIBRATION_SECONDS  10
#endif

void MPU6050_begin();
void MPU6050_probe();
void MPU6050_calibrate();

extern float Tmp;
extern double acceleroX, acceleroY, acceleroZ;

#endif
