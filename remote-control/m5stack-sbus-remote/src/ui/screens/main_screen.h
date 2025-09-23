#pragma once

extern lv_obj_t *ui_screen;
extern lv_obj_t *ui____initial_actions0;

void ui_main_screen_init();
void ui_main_screen_destroy();

void ui_main_screen_change_tab(uint8_t index);