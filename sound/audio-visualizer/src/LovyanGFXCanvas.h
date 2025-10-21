#pragma once

#include <Color.h>
#include <Canvas.h>

class LovyanGFXCanvas : public Canvas<Color>
{
public:
	LovyanGFXCanvas(LGFX& display) : _display(&display)
	{ }

	void Init(const Color& color = Color::Black)
	{
		_display->init();
		_display->startWrite();
		_display->fillScreen((unsigned short)color);

		if (_display->isEPD())
		{
			_display->setEpdMode(epd_mode_t::epd_fastest);
		}
		if (_display->width() < _display->height())
		{
			_display->setRotation(_display->getRotation() ^ 1);
		}
	}

	void LoadFont(const uint8_t array[])
	{
		_display->loadFont(array);
	}

	void DrawPoint(int x, int y, const Color& color)
	{
		_display->drawPixel(x, y, (unsigned short)color);
	}

	void DrawLine(int x0, int y0, int x1, int y1, const Color& color)
	{
		_display->drawLine(x0, y0, x1, y1, (unsigned short)color);
	}

	void DrawFilledRect(int x0, int y0, int w, int h, const Color& color)
	{
		_display->fillRect(x0, y0, w, h, (unsigned short)color);
	}

	 void DrawRect(int x0, int y0, int w, int h, const Color& color)
	 {
			_display->drawRect(x0, y0, w, h, (unsigned short)color);
	 }

	 void DrawImage(int x, int y, int w, int h, const unsigned short* data)
	 {
			_display->setSwapBytes(true);
			_display->pushImage(x, y, w, h, data); //, (uint16_t)_background
			_display->setSwapBytes(false);
	 }

	 void DrawText(int x, int y, int w, int h, const char* text, const Color& color)
   {
			_display->setTextColor((unsigned short)color);
			_display->drawCenterString(text, x + w/2, y + h/6);
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
		return _display->height();
	}

	inline int Width() const
	{
		return _display->width();
	}

	inline unsigned int Bpp() const
	{
		return 16;
	}

	inline void Clear(const Color& color)
	{
		_display->fillScreen((unsigned short)color);
	}

	inline void Clear()
	{
		Clear(_background);
	}

	inline void Update()
	{
			_display->display();
	}

private:
	LGFX* _display;

	const Color _background = Color::Black;
};

