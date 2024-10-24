#include "lvgl.h"
#include "app_hal.h"

#include "ui/ui.h"

static void timer_task(lv_timer_t *t)
{
    ui_set_altitude(hal_get_altitude());
    ui_set_pitch(hal_get_pitch());
}

#ifdef ARDUINO

#include "OneButton.h"

OneButton button1(PIN_BUTTON_1, true);
OneButton button2(PIN_BUTTON_2, true);

void setup()
{ 
  lv_init();

  hal_setup();

  ui_init();

  lv_timer_t *timer = lv_timer_create(timer_task, 500, NULL);

  button1.attachClick([]() {
      ui_show(0, true);
  });

  button2.attachClick([]() {
      ui_show(0, false);
  });
}

void loop()
{
  hal_loop();
  button1.tick();
  button2.tick();
}

#else

int main(void)
{
	lv_init();

	hal_setup();

  ui_init();

  lv_timer_t *timer = lv_timer_create(timer_task, 500, NULL);

  while(1) {
    hal_loop();
  }
}

#endif