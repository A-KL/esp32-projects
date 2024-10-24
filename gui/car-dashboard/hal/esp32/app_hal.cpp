#include <stdint.h>
#include <cstdlib> // rnd

#include <lvgl.h>
#include <Arduino.h>
#include <Wire.h>

#include "rm67162.h"
#include "Adafruit_BME280.h"
#include "app_hal.h"

#define I2C_SDA 32
#define I2C_SCL 33
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_ADDR 0x76

Adafruit_BME280 bme;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
static TaskHandle_t lvgl_tick_task = NULL;

void hal_display_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

    lv_disp_flush_ready(disp);
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


int hal_get_altitude()
{
  return rand()%1000 + 2000;
}

int hal_get_pitch()
{
  return rand()%10 + 5;;
}

void hal_setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...\n");

  rm67162_init();
  lcd_setRotation(1);

  buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE);
  assert(buf);

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = hal_display_flush_cb;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

  xTaskCreate(hal_timer_tick, "lv_tick_thread", 2048, NULL, tskIDLE_PRIORITY, &lvgl_tick_task);

  auto status = bme.begin(BME280_ADDR);

  //lv_tick_set_cb(hal_lvgl_timer_tick_get_cb);

  lv_log_register_print_cb(hal_log_cb);
}

void hal_loop(void)
{
  delay(2);
  lv_timer_handler();
}

