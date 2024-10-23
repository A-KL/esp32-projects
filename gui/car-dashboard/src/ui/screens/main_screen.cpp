// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

#include <string>

const uint16_t separator_size = 4;
const uint16_t separator_width = 160;
const uint16_t bottom_height = 50;
const uint16_t highlight_padding = 10;

const int background_color = 0x000000;
const int background_light_color = 0x00F34F;

const int primary_color = 0xFFFFFF;
const int secondary_color = 0x969696;
const int hi_color = 0xED001E;

static int altitude_max = 0;
static int pitch_max = 0;

void ui_set_altitude(int value)
{
    auto string_value = std::to_string(value);
    lv_label_set_text(ui_alt, string_value.c_str());

    if (value > altitude_max) {
        lv_obj_set_style_bg_color(ui_alt_background, lv_color_hex(background_light_color), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(ui_alt_unit, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );

        lv_label_set_text(ui_alt_max, string_value.c_str());
        altitude_max = value;
    }
    else {
        lv_obj_set_style_bg_color(ui_alt_background, lv_color_hex(background_color), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(ui_alt_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    }
}

void ui_set_pitch(int value)
{
    auto string_value = std::to_string(value);
    lv_label_set_text(ui_pitch, string_value.c_str());

    if (value > pitch_max) {
        lv_obj_set_style_bg_color(ui_pitch_background, lv_color_hex(background_light_color), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(ui_pitch_unit, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );

        lv_label_set_text(ui_pitch_max, string_value.c_str());
        pitch_max = value;
    }
    else {
        lv_obj_set_style_bg_color(ui_pitch_background, lv_color_hex(background_color), LV_PART_MAIN | LV_STATE_DEFAULT );
        lv_obj_set_style_text_color(ui_pitch_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    }
}

uint16_t ui_get_text_width(const char* text, const int size){
   // printf("%s - %d\r\n", text, size);
    return lv_txt_get_width(text, size/2, &ui_font_conthraxfont26, 2, LV_PART_MAIN| LV_STATE_DEFAULT);
}

void ui_main_screen_init(void)
{
    auto max_alt_text = "3500";
    auto alt_unit_text = "m";

    auto max_pitch_text = "-10";
    auto pitch_unit_text = "°";

    // Main screen
    ui_screen = lv_obj_create(NULL);
    lv_obj_clear_flag( ui_screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(ui_screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    auto screen_width = LV_HOR_RES;
    auto screen_hight = LV_VER_RES;
    auto screen_center = (screen_width - separator_size) / 2;

    // Pitch Max
    auto right_center = screen_center / 2;
    auto ui_pitch_max_width = ui_get_text_width(max_pitch_text, sizeof(max_pitch_text)) / 2;
    auto ui_pitch_max_unit_width = ui_get_text_width(pitch_unit_text, sizeof(pitch_unit_text));

    ui_pitch_max = lv_label_create(ui_screen);
    lv_obj_set_width( ui_pitch_max, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_pitch_max, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_pitch_max, -(right_center - ui_pitch_max_width) );
    lv_obj_set_y( ui_pitch_max, -12 );
    lv_obj_set_align( ui_pitch_max, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_pitch_max, max_pitch_text);
    lv_obj_set_style_text_color(ui_pitch_max, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_pitch_max, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_pitch_max, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    ui_pitch_max_unit = lv_label_create(ui_screen);
    lv_obj_set_width( ui_pitch_max_unit, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_pitch_max_unit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_pitch_max_unit, -(right_center - ui_pitch_max_width - ui_pitch_max_unit_width)  );
    lv_obj_set_y( ui_pitch_max_unit, -12 );
    lv_obj_set_align( ui_pitch_max_unit, LV_ALIGN_BOTTOM_RIGHT);
    lv_label_set_text(ui_pitch_max_unit, pitch_unit_text);
    lv_obj_set_style_text_color(ui_pitch_max_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_pitch_max_unit, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_pitch_max_unit, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Alt Max
    auto left_center = screen_center / 2;
    auto ui_alt_max_width = ui_get_text_width(max_alt_text, sizeof(max_alt_text));
    auto ui_alt_max_unit_width = ui_get_text_width(alt_unit_text, sizeof(alt_unit_text));

    ui_alt_max = lv_label_create(ui_screen);
    lv_obj_set_width( ui_alt_max, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_alt_max, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_alt_max, (left_center - ui_alt_max_width));
    lv_obj_set_y( ui_alt_max, -12 );
    lv_obj_set_align( ui_alt_max, LV_ALIGN_BOTTOM_LEFT);
    lv_label_set_text(ui_alt_max, max_alt_text);
    lv_obj_set_style_text_color(ui_alt_max, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_alt_max, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_alt_max, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    ui_alt_max_unit = lv_label_create(ui_screen);
    lv_obj_set_width( ui_alt_max_unit, LV_SIZE_CONTENT);
    lv_obj_set_height( ui_alt_max_unit, LV_SIZE_CONTENT);
    lv_obj_set_x( ui_alt_max_unit, (left_center + ui_alt_max_width) );
    lv_obj_set_y( ui_alt_max_unit, -12 );
    lv_obj_set_align( ui_alt_max_unit, LV_ALIGN_BOTTOM_LEFT );
    lv_label_set_text(ui_alt_max_unit, alt_unit_text);
    lv_obj_set_style_text_color(ui_alt_max_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_font(ui_alt_max_unit, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Pitch background
    ui_pitch_background = lv_obj_create(ui_screen);
    lv_obj_remove_style_all(ui_pitch_background);
    lv_obj_set_width( ui_pitch_background, screen_center - highlight_padding * 2);
    lv_obj_set_height( ui_pitch_background, (screen_hight - bottom_height - highlight_padding * 2));
    lv_obj_set_x( ui_pitch_background, -highlight_padding );
    lv_obj_set_y( ui_pitch_background, highlight_padding );
    lv_obj_set_align( ui_pitch_background, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag( ui_pitch_background, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_radius(ui_pitch_background, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_pitch_background, lv_color_hex(background_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_pitch_background, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Pitch
    ui_pitch = lv_label_create(ui_pitch_background);
    lv_obj_set_width( ui_pitch, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_pitch, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_pitch, -5 );
    lv_obj_set_y( ui_pitch, -5 );
    lv_obj_set_align( ui_pitch, LV_ALIGN_CENTER );
    lv_label_set_text(ui_pitch,"-20");
    lv_obj_set_style_text_color(ui_pitch, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_pitch, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_pitch, &ui_font_conthraxfont48, LV_PART_MAIN| LV_STATE_DEFAULT);

    ui_pitch_unit = lv_label_create(ui_pitch_background);
    lv_obj_set_width( ui_pitch_unit, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_pitch_unit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_pitch_unit, 59 );
    lv_obj_set_y( ui_pitch_unit, -20 );
    lv_obj_set_align( ui_pitch_unit, LV_ALIGN_CENTER );
    lv_label_set_text(ui_pitch_unit,"°");
    lv_obj_set_style_text_color(ui_pitch_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_pitch_unit, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_pitch_unit, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Alt background
    ui_alt_background = lv_obj_create(ui_screen);
    lv_obj_remove_style_all(ui_alt_background);
    lv_obj_set_width( ui_alt_background, screen_center - highlight_padding * 2);
    lv_obj_set_height( ui_alt_background, (screen_hight - bottom_height - highlight_padding * 2));
    lv_obj_set_x( ui_alt_background, highlight_padding);
    lv_obj_set_y( ui_alt_background, highlight_padding);
    lv_obj_set_align( ui_alt_background, LV_ALIGN_TOP_LEFT );
    lv_obj_clear_flag( ui_alt_background, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_radius(ui_alt_background, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_alt_background, lv_color_hex(background_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(ui_alt_background, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    
    // Altimeter
    ui_alt = lv_label_create(ui_alt_background);
    lv_obj_set_width( ui_alt, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_alt, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( ui_alt, LV_ALIGN_CENTER );
    lv_label_set_text(ui_alt,"3120");
    lv_obj_set_style_text_color(ui_alt, lv_color_hex(primary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_alt, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_alt, &ui_font_conthraxfont48, LV_PART_MAIN| LV_STATE_DEFAULT);

    ui_alt_unit = lv_label_create(ui_alt_background);
    lv_obj_set_width( ui_alt_unit, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( ui_alt_unit, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( ui_alt_unit, 95 );
    lv_obj_set_y( ui_alt_unit, 5 );
    lv_obj_set_align( ui_alt_unit, LV_ALIGN_CENTER );
    lv_label_set_text(ui_alt_unit,"m");
    lv_obj_set_style_text_color(ui_alt_unit, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_text_opa(ui_alt_unit, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_alt_unit, &ui_font_conthraxfont26, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Styles
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, 0);

    static lv_grad_dsc_t grad_dsc;
    grad_dsc.dir = LV_GRAD_DIR_HOR;
    grad_dsc.stops_count = 4;

    grad_dsc.stops[0].color = lv_color_hex(background_color);
    grad_dsc.stops[0].frac = 0;

    grad_dsc.stops[1].color = lv_color_hex(secondary_color);
    grad_dsc.stops[1].frac = 60;

    grad_dsc.stops[2].color = lv_color_hex(secondary_color);
    grad_dsc.stops[2].frac = 195;

    grad_dsc.stops[3].color = lv_color_hex(background_color);
    grad_dsc.stops[3].frac = 255;

    lv_style_set_bg_grad(&style, &grad_dsc);

    // Left gradient
    ui_left_hor_separator = lv_obj_create(ui_screen);
    lv_obj_set_width( ui_left_hor_separator, separator_width);
    lv_obj_set_height( ui_left_hor_separator, separator_size);
    lv_obj_set_x( ui_left_hor_separator, (screen_center/2 - separator_width/2));
    lv_obj_set_y( ui_left_hor_separator, -bottom_height);
    lv_obj_set_align( ui_left_hor_separator, LV_ALIGN_BOTTOM_LEFT );
    lv_obj_clear_flag( ui_left_hor_separator, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_add_style(ui_left_hor_separator, &style, 0);

    // Vertical line
    // ui_vertical_separator = lv_obj_create(ui_screen);
    // lv_obj_set_width( ui_vertical_separator, separator_size);
    // lv_obj_set_height( ui_vertical_separator, lv_pct(100));
    // lv_obj_set_x( ui_vertical_separator, 0 );
    // lv_obj_set_y( ui_vertical_separator, 0 );
    // lv_obj_set_align( ui_vertical_separator, LV_ALIGN_CENTER );
    // lv_obj_clear_flag( ui_vertical_separator, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    // lv_obj_set_style_bg_color(ui_vertical_separator, lv_color_hex(secondary_color), LV_PART_MAIN | LV_STATE_DEFAULT );
    // lv_obj_set_style_bg_opa(ui_vertical_separator, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    // Right separator
    ui_right_hor_separator = lv_obj_create(ui_screen);
    lv_obj_set_width( ui_right_hor_separator, separator_width);
    lv_obj_set_height( ui_right_hor_separator, separator_size);
    lv_obj_set_x( ui_right_hor_separator, (screen_center + screen_center/2 - separator_width/2) );
    lv_obj_set_y( ui_right_hor_separator, -bottom_height );
    lv_obj_set_align( ui_right_hor_separator, LV_ALIGN_BOTTOM_LEFT );
    lv_obj_clear_flag( ui_right_hor_separator, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_add_style(ui_right_hor_separator, &style, 0);
}
