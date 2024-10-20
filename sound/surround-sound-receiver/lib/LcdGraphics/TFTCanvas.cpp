#include "TFTCanvas.h"

#include "Free_Fonts.h"
#include "FreeSansBold16pt7b.h"

#include "Orbitron_Bold_12.h"
#include "Orbitron_Regular9pt7b.h"
#include "Orbitron_Regular11pt7b.h"

#include "Prototype9pt7b.h"
#include "Prototype11pt7b.h"

#include "DejaVu_Sans.h"

TFTCanvas::TFTCanvas() : 
  _display(), _background(0, 0, 0)
{ }

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

void TFTCanvas::SetFont(int index, unsigned char size)
{
  if (index == 0) {
    _display.setFreeFont(&DejaVu_Sans_Bold_11);
  }
  else {
    _display.setFreeFont(&FreeSansBold16pt7b);
  }

 // _display.setTextSize(size);
 _display.setFreeFont(&DejaVu_Sans_Bold_11);
}

void TFTCanvas::DrawText(int x, int y, int w, int h, const char* text, const Color& color)
{
  _display.setTextColor((unsigned short)color);

  int16_t  x0=0, y0=0;
  int16_t  x1, y1;
  uint16_t w1, h1;

  w1 = _display.textWidth(text);
  h1 = _display.fontHeight();

  if (w > w1)
  {
    x += (w - w1) / 2.0;
  }

  if (h > h1)
  {
    y += (h - h1) / 2.0;
  }

  _display.drawString(text, x, y + 2, GFXFF);
}

void TFTCanvas::DrawImage(int x, int y, int w, int h, const unsigned short* data)
{
    // Swap the colour byte order when rendering
  _display.setSwapBytes(true);
  _display.pushImage(x, y, w, h, (const uint16_t *)data);
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
