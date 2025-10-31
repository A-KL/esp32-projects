#pragma once

#include <Arduino.h>
#include <Color.h>
#include <Canvas.h>

class TFT_eSPI_Canvas : public Canvas<Color>
{
public:
	TFT_eSPI_Canvas(TFT_eSPI* tft) : _tft(tft), _background(0, 0, 0)
	{}

	void Init(const Color& color)
	{
		_tft->begin();
		
		if (_tft->width() < _tft->height())
		{
			_tft->setRotation(_tft->getRotation() ^ 1);
		}

		_tft->fillScreen((unsigned short)color);
	}

	void DrawPoint(int x, int y, const Color& color)
	{
			_tft->drawPixel(x, y, (unsigned short)color);
	}

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color)
	{
			_tft->drawLine(x0, y0, x1, y1, (unsigned short)color);
	}

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
	{
			_tft->fillRect(x0, y0, w, h, (unsigned short)color);
	}

	void DrawRect(int x0, int y0, int w, int h, const Color& color)
	{
			_tft->drawRect(x0, y0, w, h, (unsigned short)color);
	}

	void LoadFont(const uint8_t* array)
	{	
	#ifdef LOVYANGFX_HPP_
			_tft->loadFont(array);
	#endif
	}

	void DrawImage(int x, int y, int w, int h, const unsigned short* data)
	{
	#ifdef LOVYANGFX_HPP_
		_tft->drawBitmap(x, y, (uint8_t*)data, w, h, (uint16_t)_background);
	#else
		_tft->drawRGBBitmap(x, y, data, w, h);
	#endif
	}

	void DrawText(int x, int y, int w, int h, const char* text, const Color& color)
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
	
	void SetFont(int index, unsigned char size)
	{
			// if (index == 0) {
			//   _tft->setFreeFont(1);
			// }
			// else {
			//   _tft->setFreeFont(index);
			// }

			// _tft->setTextSize(size);
	}

	inline int Height() const
	{
		return TFT_HEIGHT;
	}

	inline int Width() const
	{
		return TFT_WIDTH;
	}

	void Clear(const Color& color)
	{
			_tft->fillScreen((unsigned short)color);
	}

	void Clear()
	{
		Clear(_background);
	}

	void Update()
	{
		//_display.display();
	}

	virtual ~TFT_eSPI_Canvas()
	{ }

private:
	TFT_eSPI* _tft;

	const Color _background;
};

