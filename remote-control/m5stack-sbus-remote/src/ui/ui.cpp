//#include "lvgl.h"

#include "ui.h"
#include "ui_helpers.h"
#include "screens/main_screen.h"

lv_theme_t * ui_dark_theme()
{
    return lv_theme_default_init(lv_disp_get_default(), lv_color_hex(0xff0000), lv_color_hex(0x00ff00), true, LV_FONT_DEFAULT);
}

void ui_init()
{
    lv_disp_t *dispp = lv_disp_get_default();

    //lv_theme_t *theme = lv_theme_basic_init(dispp);
    lv_theme_t * theme = ui_dark_theme();

    lv_disp_set_theme(dispp, theme);

    ui_main_screen_init();

    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_screen);
}

void ui_destroy()
{
    ui_main_screen_destroy();
}

void ui_select_tab(int index)
{
    ui_main_screen_change_tab(index);
}