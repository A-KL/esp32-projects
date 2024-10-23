#include <stdint.h>
#include <Arduino.h>
#include "lvgl.h"
#include "rm67162.h"

#include "app_hal.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

void hal_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

    lv_disp_flush_ready(disp);
}

// static void timer_task(lv_timer_t *t)
// {
//     lv_obj_t *seg = (lv_obj_t *)t->user_data;
//     static bool j;
//     if (j)
//         lv_obj_add_flag(seg, LV_OBJ_FLAG_HIDDEN);
//     else
//         lv_obj_clear_flag(seg, LV_OBJ_FLAG_HIDDEN);
//     j = !j;
// }

int hal_get_altitude()
{
  return 4100;
}

int hal_get_pitch()
{
  return 10;
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
  disp_drv.flush_cb = hal_disp_flush;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

  //lv_timer_t *timer = lv_timer_create(timer_task, 500, seg_text);

  //lcd_fill(0, 0, 536, 240, 0xFFF0);
  //lcd_fill(0, 0, EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES, 0xFF00);
}

void hal_loop(void)
{
  delay(2);
  lv_timer_handler();
}