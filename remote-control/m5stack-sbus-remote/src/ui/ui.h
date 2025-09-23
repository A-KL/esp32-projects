#pragma once

#include <stdio.h>

void ui_init(void);
void ui_destroy(void);

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