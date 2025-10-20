#pragma once

#include "st7789.h"
#include <lvgl.h>

void hal_display_flush_ready_cb(void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
}

void lv_lcd_init(uint32_t color = 0x0000)
{
  //lcd_init(hal_display_flush_ready_cb, &disp_drv);
}

void lv_lcd_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
  lcd_display(disp_drv->user_data, area->x1, area->y1, area->x2, area->y2, (uint16_t *)&color_p->full);
}