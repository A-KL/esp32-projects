#include "Adafruit_GFX.h"

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#include "../Fonts/FreeSans7pt7b.h"
#include "../Fonts/FreeSans8pt7b.h"
#include "../Fonts/FreeSansBold7pt7b.h"
#include "../Fonts/FreeSansBold8pt7b.h"


#include "../BaseGraphics/Color.h"

#include "TFTCanvas.h"

void TFTCanvas::Init(const Color& color)
{
  _display.begin();
  
  if (_display.width() < _display.height())
  {
    _display.setRotation(_display.getRotation() ^ 1);
  }

  _display.fillScreen((unsigned short)color);
}

void TFTCanvas::DrawPoint(int x, int y, const Color& color)
{
	_display.drawPixel(x, y, (unsigned short)color);
}

void TFTCanvas::DrawLine(int x0, int y0, int x1, int y1, const Color& color)
{
	_display.drawLine(x0, y0, x1, y1, (unsigned short)color);
}

void TFTCanvas::DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
{
	_display.fillRect(x0, y0, w, h, (unsigned short)color);
}

void TFTCanvas::DrawRect(int x0, int y0, int w, int h, const Color& color)
{
  _display.drawRect(x0, y0, w, h, (unsigned short)color);
}

void TFTCanvas::SetFont(const char* fontName, unsigned char size)
{
  _display.setFont(&FreeSansBold7pt7b);
  _display.setTextSize(size);
}

void TFTCanvas::DrawText(int x, int y, int w, int h, const char* text, const Color& color)
{
  _display.setTextColor((unsigned short)color);

  int16_t  x0=0, y0=0;
  int16_t  x1, y1;
  uint16_t w1, h1;

  _display.getTextBounds(text, x, y, &x1, &y1, &w1, &h1);

  if (w > w1)
  {
    x += (w - w1) / 2.0;
  }

  // if (h > h1)
  // {
  //   y += (h - h1) / 2.0;
  // }

  _display.setCursor(x, y);

	_display.println(text);
}

void TFTCanvas::DrawImage(int x, int y, int w, int h, const unsigned short* data)
{
  _display.drawRGBBitmap(x, y, data, w, h);
}

void TFTCanvas::Clear(const Color& color)
{
	_display.fillScreen((unsigned short)color);
}

void TFTCanvas::Clear()
{
  Clear(_background);
}

void TFTCanvas::Update()
{
	//_display.display();
}

TFTCanvas::~TFTCanvas()
{ }
