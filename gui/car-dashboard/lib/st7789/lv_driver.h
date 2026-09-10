#pragma once

#include <Arduino.h>

#include "st7789.h"
#include <lvgl.h>

void hal_display_flush_ready_cb(void *user_ctx)
{
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
}

void lv_lcd_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  lcd_display(disp->user_data, area->x1, area->y1, area->x2, area->y2, (uint16_t *)&color_p->full);
}

void lv_lcd_init()
{
  static lv_disp_drv_t disp_drv;
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t *buf;

  lcd_init(hal_display_flush_ready_cb, &disp_drv);

  buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE);
  assert(buf);

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  /*Initialize the display*/
  ESP_LOGI(TAG, "Register display driver to LVGL");
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = lv_lcd_flush_cb;
  disp_drv.draw_buf = &draw_buf;
  // disp_drv.full_refresh = 1;          //full_refresh must be 1
 // disp_drv.user_data = panel;
  lv_disp_drv_register(&disp_drv);
}
