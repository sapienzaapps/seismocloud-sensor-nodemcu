#ifndef __MPU6050_H
#define __MPU6050_H

#include "common.h"
#ifdef IS_ESP

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDRESS   0x68
#define WIRE_SDA      D1
#define WIRE_SCL      D2

class AcceleroMPU6050
{
  public:
    AcceleroMPU6050();
    void begin();
    double getTotalVector();
    void calibrate();

};


extern float Tmp;

#endif

#endif

