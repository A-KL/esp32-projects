#pragma once

#include <stdio.h>
#include <vector>
#include <lvgl.h>

lv_obj_t* lv_ui_create_panel(lv_obj_t *parent, uint32_t color_hex, int16_t w = 100, int16_t h = 155, int16_t border = 3)
{
    auto panel = lv_obj_create(parent);
    
    lv_obj_set_width( panel, w);
    lv_obj_set_height( panel, h);

    lv_obj_set_align( panel, LV_ALIGN_CENTER );
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(panel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag( panel, LV_OBJ_FLAG_SCROLLABLE );

    lv_obj_set_style_radius(panel, 6,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_color(panel, lv_color_hex(color_hex), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(panel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, border, LV_PART_MAIN| LV_STATE_DEFAULT);

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



template <std::size_t TSize>
struct ui_panel_widget {
    lv_obj_t* content;
    lv_obj_t* title;

    lv_obj_t* labels[TSize];
    lv_obj_t* values[TSize];
};

template <std::size_t TSize>
class WidgetPanel
{
    public:
        // WidgetPanel(lv_obj_t* parent) : _parent(parent)
        // { }

         template<typename... Args>
         void setText(int index, const char* format, Args... args) {
            // char buff[15];
            // snprintf(buff, sizeof(buff), format, args...);
            // _list[index] = buff;
         }

        ~WidgetPanel() 
         { }

    private:
         lv_obj_t* _parent;
         lv_obj_t* _panel;
         lv_obj_t* _panel_title;

         lv_obj_t* _labels[TSize];
         lv_obj_t* _values[TSize];
};
