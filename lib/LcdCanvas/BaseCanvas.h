#pragma once

#include <Canvas.h>

template<typename TDisplay, typename TColor>
class BaseCanvas : public Canvas<TColor>
{
  public:
    BaseCanvas(TDisplay* display) : _display(display), _background(TColor::Black)
    {}

    /* Draw API */

    void DrawPoint(int x, int y, const TColor& color)
    {
      _display->drawPixel(x, y, (unsigned short)color);
    }

    void DrawLine(int x0, int y0, int x1, int y1, const TColor& color) {
      if (y0 == y1) {
        _display->drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
      } else if (x0 == x1) {
        _display->drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
      } else {
        _display->drawLine(x0, y0, x1, y1, (unsigned short)color);
      }
    }

    void DrawFilledRect(int x0, int y0, int w, int h, const TColor& color) {
      _display->fillRect(x0, y0, w, h, (unsigned short)color);
    }

    void DrawRect(int x0, int y0, int w, int h, const TColor& color) {
      _display->drawRect(x0, y0, w, h, (unsigned short)color);
    }

  	void DrawText(int x, int y, int w, int h, const char* text, const TColor& color, const TColor& colorBackground = TColor::Black) {
      _display->setTextColor((unsigned short)color, (unsigned short)colorBackground);
     // _display->drawCentreString(text, x + w/2, y + (h - _display->fontHeight() + 2) / 2);
	  }

    void DrawImage(int x, int y, int w, int h, const unsigned short* data)
    {
      _display->setSwapBytes(true);
      _display->pushImage(x, y, w, h, data);
      _display->setSwapBytes(false);
    }

    /* General */

    inline int Height() const {
      return _display->height();
    }

    inline int Width() const {
      return _display->width();
    }

    inline unsigned int Bpp() const {
      return 16;
    }

    inline void Clear(const TColor& color) {
      _display->fillScreen((unsigned short)color);
    }

    inline void Clear() {
      Clear(_background);
    }

    inline virtual void Update()
    {  }

  protected:
    TDisplay* _display;
    TColor _background;
};