#include <stdint.h>
#include <cstdlib> // rnd

#include <lvgl.h>
#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP280.h>

#ifdef LCD_USB_QSPI_DREVER
#include "rm67162.h"
#else
#include "esp_lcd.h"
#endif

#include "app_hal.h"

#define SEALEVELPRESSURE_HPA (1019)
#define BMP280_ADDR 0x76

Adafruit_BMP280 bme;
Adafruit_MPU6050 mpu;

bool bme_initialized = false;
bool mpu_initialized = false;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
static TaskHandle_t lvgl_tick_task = NULL;

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;

float read_angle()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  
  auto x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  auto y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  auto z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  return y;
}
#ifndef LCD_USB_QSPI_DREVER
void hal_display_flush_ready_cb(void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
}
#endif

void hal_display_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
#ifdef LCD_USB_QSPI_DREVER
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);
    lv_disp_flush_ready(disp);
#else
    lcd_PushColors(disp->user_data, area->x1, area->y1, area->x2, area->y2, (uint16_t *)&color_p->full);
#endif
}

static uint32_t hal_lvgl_timer_tick_get_cb(void) 
{ 
  return millis(); 
}

static void hal_timer_tick(void * pvParameters)
{
    while(1) {
        vTaskDelay(5);
        lv_tick_inc(5);
    }
}

void hal_log_cb(const char * buf)
{
  log_i("LOG: %s\r\n", buf);
  /*Send the logs via serial port*/
  // if(level == LV_LOG_LEVEL_ERROR) serial_send("ERROR: ");
  // if(level == LV_LOG_LEVEL_WARN)  serial_send("WARNING: ");
  // if(level == LV_LOG_LEVEL_INFO)  serial_send("INFO: ");
  // if(level == LV_LOG_LEVEL_TRACE) serial_send("TRACE: ");

  // serial_send("File: ");
  // serial_send(file);

  // char line_str[8];
  // sprintf(line_str,"%d", line);
  // serial_send("#");
  // serial_send(line_str);

  // serial_send(": ");
  // serial_send(fn_name);
  // serial_send(": ");
  // serial_send(dsc);
  // serial_send("\n");
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

  return rand()%10 + 5;;
}

  static lv_disp_drv_t disp_drv;

void hal_setup(void)
{
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  delay(1000);
  
#ifdef LCD_USB_QSPI_DREVER
  rm67162_init();
  lcd_setRotation(1);
#endif

#ifndef LCD_USB_QSPI_DREVER
  lcd_init(hal_display_flush_ready_cb, &disp_drv);
#endif

  bme_initialized = bme.begin(BMP280_ADDR);//BMP280_ADDR
 // mpu_initialized = mpu.begin();

  // mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  // mpu.setMotionDetectionThreshold(1);
  // mpu.setMotionDetectionDuration(20);
  // mpu.setInterruptPinLatch(true);	// Keep it latched.  Will turn off when reinitialized.
  // mpu.setInterruptPinPolarity(true);
  // mpu.setMotionInterrupt(true);

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  mpu_initialized = true;

  buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE);
  assert(buf);

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  /*Initialize the display*/


  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = hal_display_flush_cb;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

  xTaskCreate(hal_timer_tick, "lv_tick_thread", 2048, NULL, tskIDLE_PRIORITY, &lvgl_tick_task);

  //lv_tick_set_cb(hal_lvgl_timer_tick_get_cb);

  lv_log_register_print_cb(hal_log_cb);
}

void hal_loop(void)
{
  delay(2);
  lv_timer_handler();
}

