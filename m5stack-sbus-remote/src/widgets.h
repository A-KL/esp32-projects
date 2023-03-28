#ifndef Widgets_h
#define Widgets_h

#include <string.h>
#include <M5Stack.h>

class WidgetPanel 
{
   public:
    WidgetPanel();
    

   private:
};

class WidgetLable 
{
   public:
        WidgetLable(const int x, const int y) : X(x), Y(y)
        {

        }

        const int X;
        const int Y;
    

    private:
        String _text;
        uint32_t _color;
};

#endif