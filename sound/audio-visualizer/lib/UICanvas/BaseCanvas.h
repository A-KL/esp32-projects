#pragma once

#include <Color.h>
#include <Canvas.h>

template<typename TDisplay>
class BaseCanvas : public Canvas<Color>
{
  public:
    BaseCanvas(TDisplay* display) : _display(display), _background(Color::Black)
    {}

    /* Draw API */

    void DrawLine(int x0, int y0, int x1, int y1, const Color& color) {
      if (y0 == y1) {
        _display->drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
      } else if (x0 == x1) {
        _display->drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
      } else {
        _display->drawLine(x0, y0, x1, y1, (unsigned short)color);
      }
    }

    void DrawFilledRect(int x0, int y0, int w, int h, const Color& color) {
      _display->fillRect(x0, y0, w, h, (unsigned short)color);
    }

    void DrawRect(int x0, int y0, int w, int h, const Color& color) {
      _display->drawRect(x0, y0, w, h, (unsigned short)color);
    }

  	void DrawText(int x, int y, int w, int h, const char* text, const Color& color) {
      _display->setTextColor((unsigned short)color, 0xFF);
     // _display->drawCentreString(text, x + w/2, y + (h - _display->fontHeight() + 2) / 2);
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

    inline void Clear(const Color& color) {
      _display->fillScreen((unsigned short)color);
    }

    inline void Clear() {
      Clear(_background);
    }

  protected:
    TDisplay* _display;
    Color _background;
};