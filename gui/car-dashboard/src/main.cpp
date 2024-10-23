/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <cstdlib> // rnd
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

#ifdef ARDUINO

void setup()
{ 
  lv_init();

  hal_setup();

  ui_init();
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