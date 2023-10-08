#ifndef Widgets_h
#define Widgets_h

#include <string.h>
#include <stdio.h>
#include <TFT_eSPI.h>

const int corner_radius = 0;
const int widget_width = 100;
const int widget_title_height = 16;

const int text_margin_y = 2;
const int text_margin_x = 8;

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
               
         return ret;
      }

      void renderText(TFT_eSprite& canvas, const int16_t x, const int16_t y, const uint32_t color, const char* text) 
      {
         canvas.setTextColor(color565(color));
         canvas.drawString(text, x, y);
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
         canvas.createSprite(Width, Height);

         renderRoundedSmooth(canvas, color565(_title), color565(_background));

         renderText(canvas, text_margin_x, text_margin_y, COLOR_LIGHT_GRAY, _text);

         canvas.pushSprite(Left, Top);
         canvas.deleteSprite();
      }

      const static int Small;
      const static int Medium;
      const static int Large;
      const static int ExtraLarge;

   private:
      const char* _text;
      uint32_t _background;
      uint32_t _title;

      void renderNormal(TFT_eSprite& canvas)
      {
         canvas.fillRoundRect(0, 0, Width, Height, corner_radius, color565(_background));
         canvas.fillRoundRect(0, 0, Width, widget_title_height, corner_radius, color565(_title));
      }

      void renderRoundedSmooth(TFT_eSprite& canvas, int32_t color_title, int32_t color_tile)
      {
         int32_t radius = 8;

         int32_t x = 0;
         int32_t y = 0;

         int32_t title_padding = radius * 1.5;

         // canvas.setTextColor(TFT_BLACK, color_title);

         canvas.fillSmoothCircle(x + radius, y + radius, radius, color_title);

         canvas.fillSmoothCircle(x + Width - radius - 1, y + radius, radius, color_title);

         canvas.fillRect(x + radius, y, Width - radius * 2, radius, color_title);

         canvas.fillRect(x, y + radius, Width, title_padding, color_title);

         //canvas.drawString(title, radius, 4);

         canvas.fillRect(x, y + radius + title_padding, Width, Height - radius * 2 - title_padding, color_tile);

         canvas.fillSmoothCircle(x + radius, y + Height - radius, radius, color_tile);

         canvas.fillSmoothCircle(x + Width - radius - 1, y + Height - radius, radius, color_tile);

         canvas.fillRect(x + radius, y + Height - radius, Width - radius * 2, radius, color_tile);
      }
};

template <std::size_t TSize>
class WidgetList : public WidgetRect
{
   public:
        WidgetList(const int left = 0, const int top = 0, const int width = widget_width, const int height = WidgetPanel::Medium, const uint32_t background = TFT_TRANSPARENT, const uint32_t color = COLOR_LIGHT_GRAY) 
            : WidgetRect(left, top, width, height), _margin_x(text_margin_x), _margin_y(text_margin_y), _background(background), _color(color)
        {}

        WidgetList(const WidgetRect& parent, const int margin_left = 0, const int margin_top = 0, const uint32_t background = TFT_TRANSPARENT, const uint32_t color = COLOR_LIGHT_GRAY) 
            : WidgetList(parent.Left + margin_left, parent.Top + margin_top, parent.Width - margin_left * 2, parent.Height - margin_top, background, color)
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
         canvas.createSprite(Width, Height);
         canvas.fillSprite(color565(_background));

         for (auto i = 0; i < TSize; ++i) 
         {
            renderText(canvas, _margin_x, _margin_y + _margin_x / 2 + i * 15, _color, _list[i].c_str());
         }

         canvas.pushSprite(Left, Top);
         canvas.deleteSprite();
      }

    private:
        const int _margin_x;
        const int _margin_y;
        const uint32_t _color;
        const uint32_t _background;
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

const int WidgetPanel::Small = widget_s_height;
const int WidgetPanel::Medium = widget_m_height;
const int WidgetPanel::Large = widget_l_height;
const int WidgetPanel::ExtraLarge = widget_xl_height;

#endif