/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "lvgl.h"
#include "app_hal.h"

#include "ui/ui.h"

// static int ui_update_thread(void * data)
// {
//     (void)data;

//     while(1) {
//         delay(1000);
//         ui_set_altitude(rand()%1000 + 2000);
//     }

//     return 0;
// }

static void timer_task(lv_timer_t *t)
{
    // lv_obj_t *seg = (lv_obj_t *)t->user_data;
    // static bool j;
    // if (j)
    //     lv_obj_add_flag(seg, LV_OBJ_FLAG_HIDDEN);
    // else
    //     lv_obj_clear_flag(seg, LV_OBJ_FLAG_HIDDEN);
    // j = !j;
    ui_set_altitude(hal_get_altitude());
    ui_set_pitch(hal_get_pitch());
}

#ifdef ARDUINO

void setup()
{ 
  lv_init();

  hal_setup();

  ui_init();

  lv_timer_t *timer = lv_timer_create(timer_task, 1000, NULL);
}

void loop()
{
  hal_loop();
}

#else

int main(void)
{
	lv_init();

	hal_setup();

  ui_init();

  //updates(ui_update_thread);

  while(1) {
    hal_loop();
  }
}

#endif