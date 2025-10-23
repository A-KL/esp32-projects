#include <LGFX_TFT_eSPI.h>
#include <LGFX_AUTODETECT.hpp>

#include "NotoSansBold15.h"
#include <Color.h>
#include <Canvas.h>

#include "TFTCanvas.h"

void TFTCanvas::Init(const Color& color)
{
  _tft->begin();
  
  if (_tft->width() < _tft->height())
  {
    _tft->setRotation(_tft->getRotation() ^ 1);
  }

  _tft->setSwapBytes(true);
  _tft->loadFont(NotoSansBold15);

  _tft->fillScreen((unsigned short)color);
}

void TFTCanvas::DrawPoint(int x, int y, const Color& color)
{
	  _tft->drawPixel(x, y, (unsigned short)color);
}

void TFTCanvas::DrawLine(int x0, int y0, int x1, int y1, const Color& color)
{
	  _tft->drawLine(x0, y0, x1, y1, (unsigned short)color);
}

void TFTCanvas::DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
{
	  _tft->fillRect(x0, y0, w, h, (unsigned short)color);
}

void TFTCanvas::DrawRect(int x0, int y0, int w, int h, const Color& color)
{
    _tft->drawRect(x0, y0, w, h, (unsigned short)color);
}

void TFTCanvas::LoadFont(const uint8_t array[])
{
    _tft->loadFont(array);
}

void TFTCanvas::SetFont(int index, unsigned char size)
{
    // if (index == 0) {
    //   _tft->setFreeFont(1);
    // }
    // else {
    //   _tft->setFreeFont(index);
    // }

    // _tft->setTextSize(size);
}

void TFTCanvas::DrawText(int x, int y, int w, int h, const char* text, const Color& color)
{
  _tft->setTextColor((unsigned short)color);

  int16_t  x0=0, y0=0;
  int16_t  x1, y1;
  uint16_t w1, h1;

  //_tft->getTextBounds(text, x0, y0, &x1, &y1, &w1, &h1);

  if (w > w1)
  {
    x += (w - w1) / 2.0;
  }

  if (h > h1)
  {
    y += (h - h1) / 2.0 - 1;
  }

  _tft->setCursor(x, y + h1);

	_tft->println(text);
}

void TFTCanvas::DrawImage(int x, int y, int w, int h, const unsigned short* data)
{
    _tft->drawBitmap(x, y, (uint8_t*)data, w, h, (uint16_t)_background);
}

void TFTCanvas::Clear(const Color& color)
{
	  _tft->fillScreen((unsigned short)color);
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
