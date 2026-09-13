#include <stdio.h>

#include <Arduino.h>
#include <Wire.h>

#include "esp_log.h"

#include "SensorQMI8658.hpp"
#include "imu_bsp.h"

#ifndef SENSOR_SDA
#define SENSOR_SDA  47
#endif

#ifndef SENSOR_SCL
#define SENSOR_SCL  48
#endif

SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;

bool imu_init(void)
{
  if(!qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, SENSOR_SDA, SENSOR_SCL))
  {
    return false;
  }

  Serial.println("qmi_will");
  ESP_LOGI("qmi","qmi_ID:%02x",qmi.getChipID());

  if (qmi.selfTestAccel()) 
  {
    Serial.println("Accelerometer self-test successful");
  }
  else 
  {
    Serial.println("Accelerometer self-test failed!");
  }

  if (qmi.selfTestGyro())
  {
    Serial.println("Gyroscope self-test successful");
  }
  else
  {
    Serial.println("Gyroscope self-test failed!");
  }

  qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_4G,SensorQMI8658::ACC_ODR_1000Hz,SensorQMI8658::LPF_MODE_0);
  qmi.configGyroscope(SensorQMI8658::GYR_RANGE_64DPS,SensorQMI8658::GYR_ODR_896_8Hz,SensorQMI8658::LPF_MODE_3);
  qmi.enableGyroscope();
  qmi.enableAccelerometer();

  // Print register configuration information
  // qmi.dumpCtrlRegister();

  return true;
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  const float run = in_max - in_min;
  if (run == 0) {
    log_e("map(): Invalid input range, min == max");
    return -1;  // AVR returns -1, SAM returns 0
  }
  const float rise = out_max - out_min;
  const float delta = x - in_min;
  return (delta * rise) / run + out_min;
}

bool imu_read_angles(float& x, float& y, float& z)
{
  imu_data_t data;

  if (!imu_get(data)) {
    return false;
  }

  const float min_val = -1;
  const float max_val = 1;

  printf("MPU: %.2f\t%.2f\t%.2f\r\n", data.accx, data.accy, data.accz);

  int xAng = fmap(data.accx, min_val, max_val, -90, 90);
  int yAng = fmap(data.accy, min_val, max_val, -90, 90);
  int zAng = fmap(data.accz, min_val, max_val, -90, 90);
  
  x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  return true;
}

bool imu_get(imu_data_t& data)
{
  memset(&data, 0, sizeof(imu_data_t));

  if (qmi.getDataReady())
  {
    if (qmi.getAccelerometer(acc.x, acc.y, acc.z)) //mg
    {
      data.accx = acc.x;
      data.accy = acc.y;
      data.accz = acc.z;
    }

    if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z)) //dps
    {
      data.gyrox = gyr.x;
      data.gyroy = gyr.y;
      data.gyroz = gyr.z;
    }
    data.temperature = qmi.getTemperature_C();

    return true;
  }
  return false;
}


// void i2c_qmi_loop_task(void *arg)
// {
//   for(;;)
//   {
//     if (qmi.getDataReady())
//     {
//       if (qmi.getAccelerometer(acc.x, acc.y, acc.z))
//       {
//         // Print to serial plotter
//         printf("ACCEL.x:%.2f,ACCEL.y:%.2f,ACCEL.z:%.2f Unit:g\n",acc.x,acc.y,acc.z);
//       }
//       if (qmi.getGyroscope(gyr.x, gyr.y, gyr.z))
//       {
//         // Print to serial plotter
//         printf("GYRO.x:%.2f,GYRO.y:%.2f,GYRO.z:%.2f Unit:degrees/sec\n",gyr.x,gyr.y,gyr.z);
//         // Serial.print(" GYRO.x:"); Serial.print(gyr.x); Serial.println(" degrees/sec");
//         // Serial.print(",GYRO.y:"); Serial.print(gyr.y); Serial.println(" degrees/sec");
//         // Serial.print(",GYRO.z:"); Serial.print(gyr.z); Serial.println(" degrees/sec");
        
//       }
//       //printf("Temperature: %.2f Unit:degrees C\n",qmi.getTemperature_C());
//     }
//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }

