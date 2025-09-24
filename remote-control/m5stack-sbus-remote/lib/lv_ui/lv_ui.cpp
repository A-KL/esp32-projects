#include "lvgl.h"
#include "lv_ui.h"
#include "lv_ui_helpers.h"

lv_obj_t *ui_screen;
lv_obj_t *ui____initial_actions0;

lv_theme_t * ui_dark_theme()
{
    return lv_theme_default_init(lv_disp_get_default(), lv_color_hex(0xff0000), lv_color_hex(0x00ff00), true, LV_FONT_DEFAULT);
}

lv_obj_t *lv_ui_create_screen(uint32_t color_hex)
{
    auto screen = lv_obj_create(NULL);

    lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(screen, lv_color_hex(color_hex), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    return screen;
}

void lv_ui_init()
{
    lv_disp_t *disp = lv_disp_get_default();

    //lv_theme_t *theme = lv_theme_basic_init(dispp);
    lv_theme_t * theme = ui_dark_theme();
    lv_disp_set_theme(disp, theme);

    ui_screen = lv_ui_create_screen(0x000000);
    ui____initial_actions0 = lv_obj_create(NULL);

    lv_ui_main_screen_init(ui_screen);
    lv_disp_load_scr(ui_screen);
}

void lv_ui_destroy()
{
    if (ui_screen) {
        lv_obj_del(ui_screen);
    }
    lv_ui_main_screen_destroy();
}
