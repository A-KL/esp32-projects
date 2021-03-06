#include "../BaseGraphics/Color.h"

#include "M5StackCanvas.h"

void M5StackCanvas::Init(const Color& color)
{
  _display.init();
  _display.startWrite();
  _display.fillScreen((unsigned short)color);

  if (_display.isEPD())
  {
    _display.setEpdMode(epd_mode_t::epd_fastest);
  }
  if (_display.width() < _display.height())
  {
    _display.setRotation(_display.getRotation() ^ 1);
  }
}

void M5StackCanvas::DrawPoint(int x, int y, const Color& color)
{
	_display.drawPixel(x, y, (unsigned short)color);
}

void M5StackCanvas::DrawLine(int x0, int y0, int x1, int y1, const Color& color)
{
	_display.drawLine(x0, y0, x1, y1, (unsigned short)color);
}

void M5StackCanvas::DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
{
	_display.fillRect(x0, y0, w, h, (unsigned short)color);
}

void M5StackCanvas::DrawText(int x, int y, int w, int h, const char* text, const Color& color)
{
  _display.setTextColor((unsigned short)color, TFT_BLACK);
  _display.setFont(&fonts::Font2);
	_display.drawString(text, x, y);
}

void M5StackCanvas::DrawImage(int x, int y, int w, int h, const unsigned short* data)
{
  _display.pushImage(x, y, w, h, data);
}

void M5StackCanvas::Clear(const Color& color)
{
	_display.fillScreen((unsigned short)color);
}

void M5StackCanvas::Clear()
{
  Clear(_background);
}

void M5StackCanvas::Update()
{
	_display.display();
}

M5StackCanvas::~M5StackCanvas()
{ }
