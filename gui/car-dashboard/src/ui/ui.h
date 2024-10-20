#pragma once

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_screen
void ui_main_screen_init(void);

extern lv_obj_t *ui_screen;

extern lv_obj_t *ui_pitch;
extern lv_obj_t *ui_pitch_unit;
extern lv_obj_t *ui_pitch_max;
extern lv_obj_t *ui_pitch_max_unit;
extern lv_obj_t *ui_pitch_background;

extern lv_obj_t *ui_alt;
extern lv_obj_t *ui_alt_unit;
extern lv_obj_t *ui_alt_max;
extern lv_obj_t *ui_alt_max_unit;
extern lv_obj_t *ui_alt_background;

extern lv_obj_t *ui_left_hor_separator;
extern lv_obj_t *ui_vertical_separator;
extern lv_obj_t *ui_right_hor_separator;

extern lv_obj_t *ui____initial_actions0;

LV_FONT_DECLARE( ui_font_conthraxfont26);
LV_FONT_DECLARE( ui_font_conthraxfont48);

void ui_init(void);