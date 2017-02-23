#ifndef __MPU6050_H
#define __MPU6050_H

#include "common.h"
#ifdef IS_ESP

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDRESS 0x68

class AcceleroMPU6050
{
  public:
    AcceleroMPU6050();
    void begin();
    int getXAccel();
    int getYAccel();
    int getZAccel();
    int getTotalVector();
    void calibrate();

};

#endif

#endif

