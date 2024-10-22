#include <stdint.h>
#include <Arduino.h>
#include "lvgl.h"
#include "rm67162.h"

#define LV_DELAY(x)                                                                                                                                  \
  do {                                                                                                                                               \
    uint32_t t = x;                                                                                                                                  \
    while (t--) {                                                                                                                                    \
      lv_timer_handler();                                                                                                                            \
      delay(1);                                                                                                                                      \
    }                                                                                                                                                \
  } while (0);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;

typedef int (*update_callback) (void *data);

void my_disp_flush(lv_disp_drv_t *disp,
                   const lv_area_t *area,
                   lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

    lv_disp_flush_ready(disp);
}

void hal_setup(void)
{
    Serial.begin(115200);
    Serial.println("T-DISPLAY-S3-AMOLED FACTORY TEST");

    //lv_timer_t *timer = lv_timer_create(timer_task, 500, seg_text);
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

void updates(update_callback fn) 
{
   // SDL_CreateThread(fn, "ui_updates", NULL);
}

void hal_loop(void)
{
    while(1) {
        vTaskDelay(5);
        lv_task_handler();
        // lv_timer_handler();
        // delay(2);
    }
}

void setup()
{
}

void loop()
{
    lv_timer_handler();
    delay(2);
}