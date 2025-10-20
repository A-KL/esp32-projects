#pragma once

#include "rm67162.h"
#include <lvgl.h>

void lv_lcd_init(uint32_t color = 0x0000)
{
  lcd_init();
  lcd_setRotation(1);
  lcd_fill(0, 0, (TFT_WIDTH-1), (TFT_HEIGHT-1), 0x0000);
}

void lv_lcd_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    lcd_display(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);
    lv_disp_flush_ready(disp_drv);
}