#include "lvgl.h"
#include "app_hal.h"

#include "ui/ui.h"

static void timer_task(lv_timer_t *t)
{
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

  lv_timer_t *timer = lv_timer_create(timer_task, 1000, NULL);

  while(1) {
    hal_loop();
  }
}

#endif