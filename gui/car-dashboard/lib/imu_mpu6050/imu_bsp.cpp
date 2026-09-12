#include <stdio.h>

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

#include "esp_log.h"
#include "imu_bsp.h"

#define MPU_ADDR 0x68

Adafruit_MPU6050 mpu;

// int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

// float read_angle()
// {
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x3B);
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU_ADDR, 14);
//   AcX=Wire.read()<<8|Wire.read();
//   AcY=Wire.read()<<8|Wire.read();
//   AcZ=Wire.read()<<8|Wire.read();
//   int xAng = map(AcX,minVal,maxVal,-90,90);
//   int yAng = map(AcY,minVal,maxVal,-90,90);
//   int zAng = map(AcZ,minVal,maxVal,-90,90);
  
//   auto x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
//   auto y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
//   auto z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

//   return y;
// }

bool imu_init(void)
{
  // Wire.beginTransmission(MPU_ADDR);
  // Wire.write(0x6B);
  // Wire.write(0);
  // Wire.endTransmission(true);
  // return true;

  if(!mpu.begin())
  {
    return false;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  return true;
}

imu_data_t imu_get(void)
{
  imu_data_t data;

  memset(&data,0,sizeof(imu_data_t));

  sensors_event_t a, g, temp;

  if (mpu.getEvent(&a, &g, &temp))
  {
    data.accx = a.acceleration.x;
    data.accy = a.acceleration.y;
    data.accz = a.acceleration.z;

    data.gyrox = a.gyro.x;
    data.gyroy = a.gyro.y;
    data.gyroz = a.gyro.z;

    data.temperature = temp.temperature;
  }
//   Wire.beginTransmission(MPU_ADDR);
//   Wire.write(0x3B);
//   Wire.endTransmission(false);
//   Wire.requestFrom(MPU_ADDR, 14);
//   AcX=Wire.read()<<8|Wire.read();
//   AcY=Wire.read()<<8|Wire.read();
//   AcZ=Wire.read()<<8|Wire.read();
//   int xAng = map(AcX,minVal,maxVal,-90,90);
//   int yAng = map(AcY,minVal,maxVal,-90,90);
//   int zAng = map(AcZ,minVal,maxVal,-90,90);
  
//   auto x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
//   auto y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
//   auto z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  return data;
}