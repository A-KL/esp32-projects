#pragma once

#include <string.h>
#include <stdio.h>
#include <TFT_eSPI.h>

#include <vector>

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

      void renderText(TFT_eSprite& canvas, const int16_t x, const int16_t y, const uint32_t color, const uint32_t background, const char* text) 
      {
         canvas.setTextColor(color565(color), color565(background), false);
         canvas.drawString(text, x, y);
      }

      template<typename... Args>
      void renderText(TFT_eSprite& canvas, const int16_t x, const int16_t y, const uint32_t color, const uint32_t background, const char* format, Args... args) 
      {
         canvas.setTextColor(color565(color), color565(background), false);
         canvas.setCursor(x, y);
         canvas.printf(format, args...);
      }
};

class WidgetRect: public Widget
{
   public:
        WidgetRect(const int left, const int top, const int width, const int height) :
            Widget(left, top), Width(width), Height(height)
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

         renderText(canvas, text_margin_x, text_margin_y, COLOR_LIGHT_GRAY, _background, _text);

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
      uint8_t _radius = 8;
      
      void renderNormal(TFT_eSprite& canvas)
      {
         canvas.fillRoundRect(0, 0, Width, Height, corner_radius, color565(_background));
         canvas.fillRoundRect(0, 0, Width, widget_title_height, corner_radius, color565(_title));
      }

      void renderRoundedSmooth(TFT_eSprite& canvas, int32_t color_title, int32_t color_tile)
      {
         int16_t x = 0;
         int16_t y = 0;
         int16_t title_padding = _radius * 1.5;

         // canvas.setTextColor(TFT_BLACK, color_title);

         canvas.fillSmoothCircle(x + _radius, y + _radius, _radius, color_title);

         canvas.fillSmoothCircle(x + Width - _radius - 1, y + _radius, _radius, color_title);

         canvas.fillRect(x + _radius, y, Width - _radius * 2, _radius, color_title);

         canvas.fillRect(x, y + _radius, Width, title_padding, color_title);

         //canvas.drawString(title, radius, 4);

         canvas.fillRect(x, y + _radius + title_padding, Width, Height - _radius * 2 - title_padding, color_tile);

         canvas.fillSmoothCircle(x + _radius, y + Height - _radius, _radius, color_tile);

         canvas.fillSmoothCircle(x + Width - _radius - 1, y + Height - _radius, _radius, color_tile);

         canvas.fillRect(x + _radius, y + Height - _radius, Width - _radius * 2, _radius, color_tile);
      }
};

template <std::size_t TSize>
class WidgetList : public WidgetRect
{
   public:
        WidgetList(const int left, const int top, const int width, const int height, const uint32_t background = TFT_TRANSPARENT, const uint32_t color = COLOR_LIGHT_GRAY) 
            : WidgetRect(left, top, width, height), _background(background), _color(color)
        {}

         WidgetList(const WidgetRect& parent, const int margin_left = text_margin_x, const int margin_top = text_margin_y, const uint32_t background = TFT_TRANSPARENT, const uint32_t color = COLOR_LIGHT_GRAY) 
            : WidgetList(
               (parent.Left + margin_left), 
               (parent.Top + margin_top), 
               (parent.Width - margin_left * 2), 
               (parent.Height - margin_top), 
               background, color)
        {}

         inline void setText(int index, String value) {
            _list[index] = value;
         }

         template<typename... Args>
         void setText(int index, const char* format, Args... args) {
            char buff[15];
            snprintf(buff, sizeof(buff), format, args...);
            _list[index] = buff;
         }

         void clear() {
            for(auto i = 0; i < TSize; ++i) {
               _list[i] = "";
            }
         }

      void render(TFT_eSprite& canvas)
      {
         canvas.createSprite(Width, Height);
         canvas.setColorDepth(16);
         canvas.setSwapBytes(true);
        // canvas.fillSprite(TFT_TRANSPARENT);
         canvas.fillSprite(color565(_background));

         for (auto i = 0; i < TSize; ++i) 
         {
            renderText(canvas, 0,  i * 15, _color, _background, _list[i].c_str());
         }
         canvas.pushSprite(Left, Top);
         canvas.deleteSprite();
      }

    private:
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

class WidgetCarousel
{
   public:
      void add(WidgetPanel* widget) {
         _widgets.push_back(widget);

         if (_widgets.size() == 1) {
            _selected_widget = _widgets.begin();
         }
      }

      void next() {
         if (_selected_widget == _widgets.end()) {
            _selected_widget = _widgets.begin();
         } 
         else {
            ++_selected_widget;
         }
      }

      void render(TFT_eSprite& canvas) {
         (*_selected_widget)->render(canvas);
      }
   
   private:
      std::vector<WidgetPanel*> _widgets;
      std::vector<WidgetPanel*>::iterator _selected_widget;
};