#pragma once

#include <Arduino.h>
#include <lvgl.h>

#ifndef LVGL_BUF_SIZE_PER
#define LVGL_BUF_SIZE_PER 10
#endif

#define LVGL_LCD_BUF_SIZE ((TFT_WIDTH*TFT_HEIGHT)/LVGL_BUF_SIZE_PER)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LVGL_LCD_BUF_SIZE];
static TaskHandle_t lv_tick_task = NULL;

static void lv_lcd_timer_tick(void * pvParameters)
{
    while(1) {
        vTaskDelay(5);
        lv_tick_inc(5);
    }
}

#if LV_USE_LOG != 0
static void lv_lcd_log_cb(const char * buf)
{
    log_i("LOG: %s\r\n", buf);
  // if(level == LV_LOG_LEVEL_ERROR) serial_send("ERROR: ");
  // if(level == LV_LOG_LEVEL_WARN)  serial_send("WARNING: ");
  // if(level == LV_LOG_LEVEL_INFO)  serial_send("INFO: ");
  // if(level == LV_LOG_LEVEL_TRACE) serial_send("TRACE: ");
}
#endif

void lv_lcd_common_init()
{
  //lv_lcd_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb(lv_lcd_log_cb);
#endif

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = TFT_HEIGHT;
  disp_drv.ver_res =  TFT_WIDTH;
  disp_drv.flush_cb = lv_lcd_display_flush_cb;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    // stc lv_indev_drv_t indev_drv;
    // lv_indev_drv_init( &indev_drv );
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = my_touchpad_read;
    // lv_indev_drv_register( &indev_drv );ati

  xTaskCreate(lv_lcd_timer_tick, "lv_tick_thread", 2048, NULL, tskIDLE_PRIORITY, &lv_tick_task);
}

void lv_lcd_loop(void)
{
  delay(2);
  lv_timer_handler();
}