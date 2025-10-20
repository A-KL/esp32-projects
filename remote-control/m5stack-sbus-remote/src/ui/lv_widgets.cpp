#include <stdio.h>
#include <vector>
#include <string>
#include <lvgl.h>

#include "lv_widgets.h"

lv_obj_t* lv_ui_create_panel(lv_obj_t *parent, uint32_t color, uint32_t bg_color, int16_t w, int16_t h, int16_t border)
{
    auto panel = lv_obj_create(parent);

    lv_obj_set_width( panel, w);
    lv_obj_set_height( panel, h);

    lv_obj_set_align( panel, LV_ALIGN_CENTER );
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag( panel, LV_OBJ_FLAG_SCROLLABLE );

    lv_obj_set_style_radius(panel, 6,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_color(panel, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(panel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, border, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(panel, lv_color_hex(bg_color), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(panel, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

    return panel;
}

lv_obj_t *lv_ui_create_panel_title(lv_obj_t *parent, const char* title, uint32_t color_hex)
{
    auto label = lv_label_create(parent);

    lv_obj_set_width(label, lv_pct(100));
    lv_obj_set_height(label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align(label, LV_ALIGN_CENTER );
    lv_label_set_text(label, title);
    lv_obj_set_style_bg_color(label, lv_color_hex(color_hex), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    return label;
}

lv_obj_t* lv_ui_create_panel_label(lv_obj_t *parent, const char *text, int w_per)
{
    auto label = lv_label_create(parent);

    lv_obj_set_width(label, lv_pct(w_per));
    lv_obj_set_height(label, LV_SIZE_CONTENT);   /// 1

    lv_obj_set_align(label, LV_ALIGN_TOP_LEFT);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);

    lv_label_set_text(label, text);

    lv_obj_set_style_pad_left(label, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

    return label;
}

lv_obj_t* lv_ui_create_panel_label_value(lv_obj_t *parent, const char *text, int w_per)
{
    auto label = lv_label_create(parent);

    lv_obj_set_width(label, lv_pct(w_per));
    lv_obj_set_height(label, LV_SIZE_CONTENT);   /// 1

    lv_obj_set_align(label, LV_ALIGN_CENTER );

    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP);
    lv_label_set_text(label, text);

    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(label, 4, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(label, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(label, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(label, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    return label;
}