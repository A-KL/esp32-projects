#pragma once

struct imu_data_t
{
  float accx;
  float accy;
  float accz;

  float gyrox;
  float gyroy;
  float gyroz;

  float temperature;
};

bool imu_init(void);
bool imu_get(imu_data_t& data);
bool imu_read_angles(float& x, float&y, float&z);