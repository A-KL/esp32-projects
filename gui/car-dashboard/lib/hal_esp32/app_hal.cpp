#include <stdint.h>
#include <cstdlib> // rnd

#include <lvgl.h>

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>

#include "app_hal.h"
#include "i2c_bsp/i2c_bsp.h"
#include "imu_bsp.h"

#define SEALEVELPRESSURE_HPA (1019)
#define BMP280_ADDR 0x76

bool bme_initialized = false;
bool mpu_initialized = false;

static Adafruit_BMP280 bme;
static TaskHandle_t lvgl_tick_task = NULL;

static void hal_timer_tick(void * pvParameters)
{
    while(1) {
        vTaskDelay(5);
        lv_tick_inc(5);
    }
}

void hal_log_cb(const char* message)
{
  log_i("LOG: %s\r\n", message);
  // if(level == LV_LOG_LEVEL_ERROR) log_e("%s\r\n", message);
  // if(level == LV_LOG_LEVEL_WARN)  log_w("%s\r\n", message);
  // if(level == LV_LOG_LEVEL_INFO)  log_i("%s\r\n", message);
  // if(level == LV_LOG_LEVEL_TRACE) log_d("%s\r\n", message);
}

float hal_get_altitude()
{
  if (bme_initialized)
  {
    auto alt =  bme.readAltitude(SEALEVELPRESSURE_HPA);
    printf("Altitude: %.2f\r\n", alt);

    auto presure =  bme.readPressure();
    printf("Pressure: %.2f\r\n", presure);

    auto temperature =  bme.readTemperature();
    printf("Temperature: %.2f\r\n", temperature);

    return alt;
  }

  return rand()%1000 + 2000;
}

float read_angle()
{
  const int minVal=265;
  const int maxVal=402;

  auto data = imu_get();

  // Wire.beginTransmission(MPU_ADDR);
  // Wire.write(0x3B);
  // Wire.endTransmission(false);
  // Wire.requestFrom(MPU_ADDR, 14);

  // AcX=Wire.read()<<8|Wire.read();
  // AcY=Wire.read()<<8|Wire.read();
  // AcZ=Wire.read()<<8|Wire.read();

  int xAng = map(data.accx, minVal, maxVal,-90,90);
  int yAng = map(data.accy, minVal, maxVal,-90,90);
  int zAng = map(data.accz, minVal, maxVal,-90,90);
  
  auto x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  auto y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  auto z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  return y;
}

float hal_get_pitch()
{
  if (mpu_initialized)
  {
    // sensors_event_t a, g, temp;
    // mpu.getEvent(&a, &g, &temp);

    // auto xAng = a.acceleration.roll;
    // auto yAng = a.acceleration.pitch;
    // auto zAng = a.acceleration.heading;

    // auto x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
    // auto y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
    // auto z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    
    // printf("MPU: %.2f\t%.2f\t%.2f\r\n", x, y, z);

    return read_angle() - 180;
  }

  return rand()%10 + 5;
}

void hal_setup(void)
{
  Serial.begin(115200);
  
  #ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(2000);
  #endif

  // Wire.begin(ESP_SDA_NUM, ESP_SCL_NUM);
  i2c_master_init();

  mpu_initialized = imu_init();
  log_i("IMU found: %s", mpu_initialized ? "YES" : "NO");

  bme_initialized = bme.begin(BMP280_ADDR);
  log_i("BMP280 found: %s", bme_initialized ? "YES" : "NO");

  xTaskCreate(hal_timer_tick, "lv_tick_thread", 2048, NULL, tskIDLE_PRIORITY, &lvgl_tick_task);
  //lv_tick_set_cb(hal_lvgl_timer_tick_get_cb);
}

void hal_loop(void)
{
  delay(2);
  lv_timer_handler();
}
