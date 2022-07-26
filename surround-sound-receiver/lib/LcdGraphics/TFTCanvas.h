#pragma once

#include "Adafruit_ILI9341.h"

//#include "TFT_eSPI.h"

#include "../BaseGraphics/Canvas.h"

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     05
#define TFT_DC     04
#define TFT_RESET  00

class TFTCanvas :
	public Canvas<Color>
{
public:
	TFTCanvas() :
		 _display(TFT_CS, TFT_DC, TFT_RESET), _background(0, 0, 0)
	{}

	void Init(const Color& color);

	void DrawPoint(int x, int y, const Color& color);

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color);

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color);

	void DrawRect(int x0, int y0, int w, int h, const Color& color);

	void DrawText(int x, int y, int w, int h, const char* text, const Color& color);

	void DrawImage(int x, int y, int w, int h, const unsigned short* data);

	void SetFont(int index, unsigned char size);

	inline int Height() const
	{
		return _display.height();
	}

	inline int Witdth() const
	{
		return _display.width();
	}

	void Clear(const Color& color);

	void Clear();

	void Update();

	virtual ~TFTCanvas();

private:
	Adafruit_ILI9341 _display;

	const Color _background;
};

