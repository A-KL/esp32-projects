#include <stdio.h>
#include <Wire.h>
#include <QMI8658.h>
#include "esp_log.h"
#include "user_config.h"
#include "imu_bsp.h"

QMI8658 imu;

// IMUdata acc;
// IMUdata gyr;

bool imu_init(void)
{
  if (!imu.begin(Wire, 0x6b)) {
    log_e("Unable to init IMU (QMI8658)");
    return false;
  }

  log_i("IMU_ID:%02x", imu.getWhoAmI());

  // Set accelerometer range (±8g)
  imu.setAccelRange(QMI8658_ACCEL_RANGE_8G);
  
  // Set accelerometer output data rate (1000Hz)
  imu.setAccelODR(QMI8658_ACCEL_ODR_1000HZ);
  
  // Set gyroscope range (±512dps)
  imu.setGyroRange(QMI8658_GYRO_RANGE_512DPS);
  
  // Set gyroscope output data rate (1000Hz)
  imu.setGyroODR(QMI8658_GYRO_ODR_1000HZ);
  
  // Set units (DEFAULT: mg for accel, dps for gyro - matches most IMU displays)
  imu.setAccelUnit_mg(true);      // Use mg (like your screen: ACC_X = -965.82)
  imu.setGyroUnit_dps(true);      // Use dps (degrees per second)
  imu.setDisplayPrecision(QMI8658_PRECISION_4);     // 4 decimal places (like your screen)

  // Enable sensors
  imu.enableSensors(QMI8658_ENABLE_ACCEL | QMI8658_ENABLE_GYRO);

  return true;
}

imu_data_t imu_get(void)
{
  imu_data_t data;
  memset(&data, 0, sizeof(imu_data_t));

  QMI8658_Data sensorData;
    
  if (imu.readSensorData(sensorData)) {
      Serial.print(millis());
      Serial.print("\t");
      Serial.print(sensorData.accelX, 6);
      Serial.print("\t");
      Serial.print(sensorData.accelY, 6);
      Serial.print("\t");
      Serial.print(sensorData.accelZ, 6);
      Serial.print("\t");
      Serial.print(sensorData.gyroX, 6);
      Serial.print("\t");
      Serial.print(sensorData.gyroY, 6);
      Serial.print("\t");
      Serial.print(sensorData.gyroZ, 6);
      Serial.print("\t");
      Serial.println(sensorData.temperature, 1);
  } else {
      Serial.println("❌ Failed to read sensor data!");
  }

  return data;
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

