#pragma once

#if defined(ARDUINO)
	#include <esp32-hal-log.h>
#else
	#define log_e(format, ...)     printf(format, ##__VA_ARGS__)
	#define log_w(format, ...)     printf(format, ##__VA_ARGS__)
	#define log_i(format, ...)     printf(format, ##__VA_ARGS__)
	#define log_d(format, ...)     printf(format, ##__VA_ARGS__)
#endif

#include <Canvas.h>

#if __has_include(<OpenFontRender.h>)
	#include "OpenFontRender.h"
	OpenFontRender ofr;
#endif

template<typename TColor>
class LovyanGFXCanvas : public Canvas<TColor>
{
public:
	LovyanGFXCanvas(TFT_eSPI* display) : _display(display), _sprite(display)
	{ }

	LovyanGFXCanvas(TFT_eSPI& display) : _display(&display), _sprite(&display)
	{ }

	bool Init(const TColor& color = TColor::Black)
	{
		if (!_display->init())
		{
			log_e("TFT ERROR\n");
			return false;
		}

		// _display->setSwapBytes(true);
		// _display->setColorDepth(16);
		_display->initDMA();
		_display->startWrite();
	
		if (_display->isEPD())
		{
			_display->setEpdMode(epd_mode_t::epd_fastest);
		}

	#ifndef TFT_ROTATION
		if (_display->width() < _display->height())
		{
			_display->setRotation(_display->getRotation() ^ 1);
		}
	#endif

		_display->fillScreen((unsigned short)color);

		log_i("TFT OK\n");

		#ifdef OPEN_FONT_RENDERER
			ofr.setDrawer(*_display);
		#endif

		return true;
	}

	void SpriteBegin(int w, int h, const TColor& color)
	{
		_sprite.createSprite(w, h);
		//_sprite.setColorDepth(16);
		//_sprite.setSwapBytes(true);
		//_sprite.fillSprite((unsigned short)color);
		//log_w("sprite %d x %d created: %d", w, h, res != nullptr);
	}

	void SpritePush(int x, int y) 
	{
			_sprite.pushSprite(x, y);
	}

	void SpriteDrawLine(int x0, int y0, int x1, int y1, const TColor& color)
	{
		if (y0 == y1) {
			_sprite.drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
		} else if (x0 == x1) {
			_sprite.drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
		} else {
			_sprite.drawLine(x0, y0, x1, y1, (unsigned short)color);
		}
	}

	void SpriteEnd()
	{
		_sprite.deleteSprite();
	}

	void DrawLine(int x0, int y0, int x1, int y1, const TColor& color)
	{
		if (y0 == y1) {
			_display->drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
		} else if (x0 == x1) {
			_display->drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
		} else {
			_display->drawLine(x0, y0, x1, y1, (unsigned short)color);
		}
	}

	void DrawFilledRect(int x0, int y0, int w, int h, const TColor& color)
	{
		_display->fillRect(x0, y0, w, h, (unsigned short)color);
	}

	void DrawRect(int x0, int y0, int w, int h, const TColor& color)
	{
		_display->drawRect(x0, y0, w, h, (unsigned short)color);
	}

	void DrawImage(int x, int y, int w, int h, const unsigned short* data)
	{
		_display->setSwapBytes(true);
		_display->pushImage(x, y, w, h, data);
		_display->setSwapBytes(false);
	}

	void DrawText(int x, int y, int w, int h, const char* text, const TColor& color, const TColor& background = TColor::Black)
	{
	#ifdef OPEN_FONT_RENDERER
		ofr.setFontSize(_fontSize);
		ofr.setCursor(x + w/2, y-2);
		ofr.setFontColor((unsigned short)color, (unsigned short)background);
		ofr.cprintf(text);
	#else
		_display->setTextColor((unsigned short)color, (unsigned short)background);
		_display->drawCentreString(text, x + w/2, y + (h - _display->fontHeight() + 2) / 2);
	#endif
	}

	void LoadFont(const uint8_t* array, const size_t size)
	{
		#if defined(OPEN_FONT_RENDERER)
			if (ofr.loadFont(array, size)) {
				log_e("Open Font Loaded: Error\n");
				return;
			}
			log_i("Open Font Loaded: OK\n");
		#else
			_display->loadFont(array);
			//_display->loadFont(NotoSansBold15);
		#endif
	}

	void SetFont(int index, unsigned char size)
	{
		#ifdef OPEN_FONT_RENDERER
		_fontSize = size;
		#endif
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

	inline void Clear(const TColor& color)
	{
		_display->fillScreen((unsigned short)color);
	}

	inline void Clear()
	{
		Clear(_background);
	}

	inline void Update()
	{
		_display->endWrite();
		_display->display();
	}

private:
	TFT_eSPI* _display;
	TFT_eSprite _sprite;
#ifdef OPEN_FONT_RENDERER
	unsigned char _fontSize = 15;
#endif
	TColor _background = TColor::Black;
};

