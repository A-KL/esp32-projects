

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

void TFTCanvas::DrawText(int x0, int y0, const char* text, int size, const Color& color)
{
  _display.setTextColor((unsigned short)color);

  //   tft.setSwapBytes(true);
  //  tft.setFreeFont(&Orbitron_Medium_20);
  _display.setTextSize(1.9f);
  _display.setCursor(x0, y0);
	_display.println(text);
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
