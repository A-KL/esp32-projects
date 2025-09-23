#pragma once

#include <lvgl.h>
#include <esp_log.h>

#ifndef LVGL_BUF_SIZE_PER
#define LVGL_BUF_SIZE_PER 10
#endif

#define LVGL_LCD_BUF_SIZE ((TFT_WIDTH*TFT_HEIGHT)/LVGL_BUF_SIZE_PER)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LVGL_LCD_BUF_SIZE];

static TaskHandle_t lv_tick_task = NULL;

#if LV_USE_LOG != 0
void hal_log_cb(const char * buf)
{
    log_i("LOG: %s\r\n", buf);
      /*Send the logs via serial port*/
  // if(level == LV_LOG_LEVEL_ERROR) serial_send("ERROR: ");
  // if(level == LV_LOG_LEVEL_WARN)  serial_send("WARNING: ");
  // if(level == LV_LOG_LEVEL_INFO)  serial_send("INFO: ");
  // if(level == LV_LOG_LEVEL_TRACE) serial_send("TRACE: ");
}
#endif

void lv_driver_init()
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb(hal_log_cb); 
#endif
}