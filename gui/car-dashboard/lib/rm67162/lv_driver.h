#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include "rm67162.h"

void lv_lcd_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd_display(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);
    lv_disp_flush_ready(disp_drv);
}

void lv_lcd_init(uint32_t color = 0x0000)
{
  lcd_init();
  lcd_setRotation(1);
  lcd_fill(0, 0, (TFT_WIDTH-1), (TFT_HEIGHT-1), color);

  static lv_disp_drv_t disp_drv;
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t *buf;

  buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE);
  assert(buf);

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  /*Initialize the display*/
  lv_disp_drv_init(&disp_drv);

  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = lv_lcd_display_flush_cb;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);
}