#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void lv_ui_main_screen_init(lv_obj_t * screen);
void lv_ui_main_screen_destroy();

void lv_ui_init(void);
void lv_ui_destroy(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif