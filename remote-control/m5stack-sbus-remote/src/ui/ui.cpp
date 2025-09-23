//#include "lvgl.h"

#include "ui.h"
#include "ui_helpers.h"

lv_obj_t *ui_screen;
lv_obj_t *ui____initial_actions0;

void ui_init( void )
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_basic_init(dispp);
    lv_disp_set_theme(dispp, theme);
    ui_main_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_screen);
}