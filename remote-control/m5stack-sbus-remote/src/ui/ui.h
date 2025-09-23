#pragma once

#include <stdio.h>

#include <lvgl.h>

//#include "ui_helpers.h"
//#include "ui_events.h"

// SCREEN: ui_screen
void ui_main_screen_init(void);

extern lv_obj_t *ui_screen;
extern lv_obj_t *ui____initial_actions0;

void ui_init(void);

template <std::size_t TSize>
class WidgetPanel
{
    public:
         template<typename... Args>
         void setText(int index, const char* format, Args... args) {
            // char buff[15];
            // snprintf(buff, sizeof(buff), format, args...);
            // _list[index] = buff;
         }
};