#ifndef Widgets_h
#define Widgets_h

#include <string.h>
#include <stdio.h>
#include <M5Stack.h>

const int corner_radius = 0;
const int widget_width = 100;
const int widget_title_height = 16;

const int text_margin_y = 12;
const int text_margin_x = 5;

const int widget_s_height = 55;
const int widget_m_height = 110;
const int widget_l_height = 170;
const int widget_xl_height = 230;

const int COLOR_LIGHT_GRAY = 0xBDBDBD;

const int COLOR_DARK_GRAY = 0x202020; // change
const int COLOR_GRAY = 0x464646; 

const int COLOR_DARK_RED = 0x3c0000;
const int COLOR_RED = 0x730000;

const int COLOR_DARK_MAGENTA = 0x3e0030; // change
const int COLOR_MAGENTA = 0x710054;

const int COLOR_DARK_GREEN = 0x092b00;
const int COLOR_GREEN = 0x155804;

const int COLOR_DARK_BLUE = 0x00252F; // change
const int COLOR_BLUE = 0x004056;

const int COLOR_DARK_YELLOW = 0x804500;
const int COLOR_YELLOW = 0xAF6700;

class Widget
{
   public:
      Widget(const int left, const int top) : 
         Left(left), Top(top)
      { }

      const int Left;
      const int Top;

   protected:
      uint16_t color565( const unsigned long rgb) 
      {
         uint16_t R = (rgb >> 16) & 0xFF;
         uint16_t G = (rgb >>  8) & 0xFF;
         uint16_t B = (rgb      ) & 0xFF;

         uint16_t ret  = (R & 0xF8) << 8;  // 5 bits
                  ret |= (G & 0xFC) << 3;  // 6 bits
                  ret |= (B & 0xF8) >> 3;  // 5 bits
               
         return( ret);
      }

      void renderText(TFT_eSprite& canvas, const int16_t x, const int16_t y, const uint32_t color, const char* text) 
      {
         canvas.setTextColor(color565(color)); 
         canvas.setCursor(x, y);
         canvas.print(text);
      }

      template<typename... Args>
      void renderText(TFT_eSprite& canvas, const int16_t x, const int16_t y, const uint32_t color, const char* format, Args... args) 
      {
         canvas.setTextColor(color565(color)); 
         canvas.setCursor(x, y);
         canvas.printf(format, args...);
      }
};

class WidgetRect: public Widget
{
   public:
        WidgetRect(const int left, const int top, const int width, const int height) :
            Widget(left, top), Width(widget_width), Height(height)
        {}

      const int Width;
      const int Height;     
};

class WidgetPanel : public WidgetRect
{
   public:
        WidgetPanel(const int left, const int top, const int size, const char* text, const uint32_t background = COLOR_DARK_GRAY, const uint32_t title = COLOR_GRAY) :
            WidgetRect(left, top, widget_width, size), _text(text), _background(background), _title(title)
        {}
    
      virtual void render(TFT_eSprite& canvas)
      {
         canvas.fillRoundRect(Left, Top, Width, Height, corner_radius, color565(_background));
         canvas.fillRoundRect(Left, Top, Width, widget_title_height, corner_radius, color565(_title));

         renderText(canvas, Left + text_margin_x, Top + text_margin_y, COLOR_LIGHT_GRAY, _text);
      }

      const static int Small;
      const static int Medium;
      const static int Large;
      const static int ExtraLarge;

   private:
      const char* _text;
      uint32_t _background;
      uint32_t _title;
};

const int WidgetPanel::Small = widget_s_height;
const int WidgetPanel::Medium = widget_m_height;
const int WidgetPanel::Large = widget_l_height;
const int WidgetPanel::ExtraLarge = widget_xl_height;

template <std::size_t TSize>
class WidgetList : public Widget
{
   public:
        WidgetList(const int left = 0, const int top = 0, const uint32_t color = COLOR_LIGHT_GRAY) 
            : Widget(left, top), _margin_x(text_margin_x), _margin_y(text_margin_y), _color(color)
        {}

        WidgetList(const Widget& parent, const int margin_left = 0, const int margin_top = 0, const uint32_t color = COLOR_LIGHT_GRAY) 
            : WidgetList(parent.Left + margin_left, parent.Top + margin_top, color)
        {}

         void setText(int index, String value) 
         {
            _list[index] = value;
         }

         template<typename... Args>
         void setText(int index, const char* format, Args... args) 
         {
            char buff[15];

            snprintf(buff, sizeof(buff), format, args...);

            _list[index] = buff;
         }

         void clear()
         {
            for(auto i = 0; i < TSize; ++i) 
            {
               _list[i] = "";
            }
         }

      void render(TFT_eSprite& canvas)
      {
         for (auto i = 0; i < TSize; ++i) 
         {
            renderText(canvas, Left + _margin_x, Top + (_margin_y + 4) * (i + 1), _color, _list[i].c_str());
         }
      }

    private:
        const int _margin_x;
        const int _margin_y;
        const uint32_t _color;
        String _list[TSize];
};

template <std::size_t TSize>
class WidgetListPanel : public WidgetPanel
{
   public:
      WidgetListPanel(const int left, const int top, const int size, const char* text, const uint32_t background = COLOR_DARK_GRAY, const uint32_t title = COLOR_GRAY) 
         : WidgetPanel(left, top, size, text, background, title), items(left, top + widget_title_height)
      {}

      WidgetList<TSize> items;

      virtual void render(TFT_eSprite& canvas)
      {
         WidgetPanel::render(canvas);
         items.render(canvas);
      }
};

#endif