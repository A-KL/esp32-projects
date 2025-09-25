#include <lvgl.h>
#include <string>

#include <lv_ui.h>
#include "lv_screen.h"

static const char* lr_labels[] = { "l:", "r:" };
static const char* acc_labels[] = { "y:", "p:", "r:" };
static const char* power_labels[] = { "V:", "mA:", "mW:" };

WidgetPanel<8> esp_now_values;
WidgetPanel<8> sbus_values;
WidgetPanel<8> nrf42_values;
WidgetPanel<8> ps_values;

WidgetPanel<4> encoder_values;
WidgetPanel<4> motor_values;

WidgetPanel<6> power_values;
WidgetPanel<3> acc_values; //TODO

lv_obj_t *ui_tab_view = NULL;

lv_obj_t *ui_tab_page_inputs = NULL;
lv_obj_t *ui_tab_page_outputs = NULL;
lv_obj_t *ui_tab_page_telemetry = NULL;

lv_obj_t *ui_create_tab(lv_obj_t *tab_view, const char* name)
{
    auto page = lv_tabview_add_tab(tab_view, name);

    lv_obj_set_style_bg_color(tab_view, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(tab_view, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_flex_flow(page,LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(page, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_left(page, 4, LV_PART_MAIN| LV_STATE_DEFAULT); //2
    lv_obj_set_style_pad_right(page, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(page, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(page, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(page, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(page, 4, LV_PART_MAIN| LV_STATE_DEFAULT);

    return page;
}

lv_obj_t *ui_create_tabview(lv_obj_t *screen, uint8_t corner_radius = 6)
{
    auto tab_view = lv_tabview_create(screen, LV_DIR_BOTTOM, 20);

    lv_obj_set_width(tab_view, lv_pct(100));
    lv_obj_set_height(tab_view, lv_pct(100));

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(lv_tabview_get_tab_btns(tab_view), lv_color_hex(0x808080),  LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(tab_view), 255,  LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(lv_tabview_get_tab_btns(tab_view), lv_color_hex(0xDEDEDE),  LV_PART_ITEMS | LV_STATE_CHECKED );
    lv_obj_set_style_text_opa(lv_tabview_get_tab_btns(tab_view), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);

    lv_obj_set_style_radius(lv_tabview_get_tab_btns(tab_view), 6,  LV_PART_ITEMS| LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(tab_view), lv_color_hex(0x545454),  LV_PART_ITEMS | LV_STATE_CHECKED );
    lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(tab_view), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);

    lv_obj_set_style_bg_main_stop(lv_tabview_get_tab_btns(tab_view), 0,  LV_PART_ITEMS| LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_stop(lv_tabview_get_tab_btns(tab_view), 255,  LV_PART_ITEMS| LV_STATE_CHECKED);

    lv_obj_set_style_border_side(lv_tabview_get_tab_btns(tab_view), LV_BORDER_SIDE_NONE,  LV_PART_ITEMS| LV_STATE_CHECKED);

    return tab_view;
}

void lv_ui_change_tab(uint8_t index)
{
     lv_tabview_set_act(ui_tab_view, index, LV_ANIM_ON);
}

void lv_ui_screen_init(lv_obj_t *ui_screen)
{
    /* TabView */

    ui_tab_view = ui_create_tabview(ui_screen);

    /* TabView Pages */

    ui_tab_page_inputs = ui_create_tab(ui_tab_view, "inputs");
    ui_tab_page_outputs = ui_create_tab(ui_tab_view, "outputs");
    ui_tab_page_telemetry = ui_create_tab(ui_tab_view, "telemetry");

    /* Page Inputs */

    sbus_values.init(ui_tab_page_inputs, "sbus", 0xD500DF, 0x000000);
    nrf42_values.init(ui_tab_page_inputs, "nrf24", 0xEF0068, 0x000000);
    esp_now_values.init(ui_tab_page_inputs, "enow", 0x585858, 0x000000);

    /* Page Outputs */

    motor_values.init(ui_tab_page_outputs, "motors", 0x00EF5E, 0x000000, 85);
    encoder_values.init(ui_tab_page_outputs, "encoders", 0xEF0035, 0x000000,  85);
    ps_values.init(ui_tab_page_outputs, "ps", 0x0000FF, 0x000000);

    /* Page Telemetry */
    
    power_values.init(ui_tab_page_telemetry, "power", 0x000255, 0x000000, 85);
    //power_values.setLabels(power_labels);
}

void lv_ui_screen_destroy()
{
    ui_tab_view = NULL;

    ui_tab_page_inputs = NULL;
    ui_tab_page_outputs = NULL;
    ui_tab_page_telemetry = NULL;
}