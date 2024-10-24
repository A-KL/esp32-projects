#pragma once

#include "lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"

// SCREEN: ui_screen
void ui_main_screen_init(void);

extern lv_obj_t *ui_screen;
extern lv_obj_t *ui____initial_actions0;

LV_FONT_DECLARE( ui_font_conthraxfont26);
LV_FONT_DECLARE( ui_font_conthraxfont48);

void ui_init(void);

void ui_show(int index, bool state);

void ui_set_altitude(float value);

void ui_set_pitch(float value);