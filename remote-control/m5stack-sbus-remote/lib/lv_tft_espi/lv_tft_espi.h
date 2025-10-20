#pragma once

#include "tft_espi_driver.h"
#include <lvgl.h>

void lv_lcd_init(uint32_t color = TFT_BLACK)
{
    lcd_init(color);
}

void lv_lcd_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    lcd_display(area->x1, area->y1, w, h, (uint16_t*) &color_p->full);

    lv_disp_flush_ready( disp_drv );
}