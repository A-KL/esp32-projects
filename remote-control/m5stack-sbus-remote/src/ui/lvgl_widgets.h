#pragma once

#include <stdio.h>
#include <vector>
#include <lvgl.h>

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

         lv_obj_t* _panel;
         lv_obj_t* _panel_title;

         std::vector<lv_obj_t*> _labels;
         std::vector<lv_obj_t*> _values;

         ~WidgetPanel() 
         {

         }
};
