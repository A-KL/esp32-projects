#pragma once

#include <string.h>
#include <stdio.h>
#include <vector>

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

WidgetPanel<8> esp_now_values;
WidgetPanel<8> sbus_values;
WidgetPanel<8> nrf42_values;
WidgetPanel<8> ps_values;

WidgetPanel<4> encoder_values;
WidgetPanel<4> motors_values;

WidgetPanel<6> power_values;

