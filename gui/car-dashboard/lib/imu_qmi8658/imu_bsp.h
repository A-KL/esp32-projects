#pragma once

typedef struct 
{
  float accx;
  float accy;
  float accz;

  float gyrox;
  float gyroy;
  float gyroz;

  float temperature;
} 
imu_data_t;

bool imu_init(void);

imu_data_t imu_get(void);