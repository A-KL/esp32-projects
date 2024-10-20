#include <stdio.h>
#include <SDL2/SDL.h>

#include "TFT_eSPI.h"
#include "TFT_eSprite.h"

TFT_eSprite::TFT_eSprite(TFT_eSPI* tft)
{
	_tft = tft;     // Pointer to tft class so we can call member functions

	_iwidth = 0; // initialise width and height to 0 (it does not exist yet)
	_iheight = 0;
	_bpp = 16;
	_swapBytes = false;   // Do not swap pushImage colour bytes by default

	_created = false;
	_vpOoB = false;

	_xs = 0;  // window bounds for pushColor
	_ys = 0;
	_xe = 0;
	_ye = 0;

	_xptr = 0; // pushColor coordinate
	_yptr = 0;

	//_colorMap = nullptr;

	//_psram_enable = true;

	//// Ensure end_tft_write() does nothing in inherited functions.
	//lockTransaction = true;
}


TFT_eSprite::~TFT_eSprite(void)
{
	deleteSprite();

#ifdef SMOOTH_FONT
	if (fontLoaded) unloadFont();
#endif
}

/***************************************************************************************
** Function name:           createSprite
** Description:             Create a sprite (bitmap) of defined width and height
***************************************************************************************/
// cast returned value to (uint8_t*) for 8-bit or (uint16_t*) for 16-bit colours
void* TFT_eSprite::createSprite(int16_t w, int16_t h, uint8_t frames)
{
	if (_created) return _surface;

	if (w < 1 || h < 1) return nullptr;

	_iwidth = _dwidth = _bitwidth = w;
	_iheight = _dheight = h;

	cursor_x = 0;
	cursor_y = 0;

	// Default scroll rectangle and gap fill colour
	_sx = 0;
	_sy = 0;
	_sw = w;
	_sh = h;
	_scolor = TFT_BLACK;

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000FF;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);

	if (_surface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		return nullptr;
	}

	//_img8 = (uint8_t*)callocSprite(w, h, frames);
	//_img8_1 = _img8;
	//_img8_2 = _img8;
	//_img = (uint16_t*)_img8;
	//_img4 = _img8;

	//if ((_bpp == 16) && (frames > 1)) {
	//	_img8_2 = _img8 + (w * h * 2 + 1);
	//}

	//// ESP32 only 16bpp check
	////if (esp_ptr_dma_capable(_img8_1)) Serial.println("DMA capable Sprite pointer _img8_1");
	////else Serial.println("Not a DMA capable Sprite pointer _img8_1");
	////if (esp_ptr_dma_capable(_img8_2)) Serial.println("DMA capable Sprite pointer _img8_2");
	////else Serial.println("Not a DMA capable Sprite pointer _img8_2");

	//if ((_bpp == 8) && (frames > 1)) {
	//	_img8_2 = _img8 + (w * h + 1);
	//}

	//// This is to make it clear what pointer size is expected to be used
	//// but casting in the user sketch is needed due to the use of void*
	//if ((_bpp == 1) && (frames > 1))
	//{
	//	w = (w + 7) & 0xFFF8;
	//	_img8_2 = _img8 + ((w >> 3) * h + 1);
	//}

	if (_surface)
	{
		_created = true;
		//if ((_bpp == 4) && (_colorMap == nullptr)) createPalette(default_4bit_palette);

		//rotation = 0;
		//setViewport(0, 0, _dwidth, _dheight);
		//setPivot(_iwidth / 2, _iheight / 2);
		return _surface;
	}

	return nullptr;
}

void TFT_eSprite::deleteSprite(void)
{
	//if (_colorMap != nullptr)
	//{
	//	free(_colorMap);
	//	_colorMap = nullptr;
	//}

	if (_created)
	{
		//free(_img8_1);
		//_img8 = nullptr;
		SDL_FreeSurface(_surface);
		_surface = NULL;
		_created = false;
		_vpOoB = true;  // TFT_eSPI class write() uses this to check for valid sprite
	}
}

/***************************************************************************************
** Function name:           pushSprite
** Description:             Push the sprite to the TFT at x, y
***************************************************************************************/
void TFT_eSprite::pushSprite(int32_t x, int32_t y)
{
	if (!_created) return;

	SDL_Rect dst{ x, y, _sw, _sh };

	SDL_Texture* texture = SDL_CreateTextureFromSurface(_tft->renderer, _surface);

	if (texture == NULL) {
		fprintf(stderr, "SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		return;
	}

	int res = SDL_RenderCopy(_tft->renderer, texture, NULL, &dst);

	SDL_DestroyTexture(texture);

	if (res < 0) {
		fprintf(stderr, "SDL_RenderCopy failed: %s\n", SDL_GetError());
		return;
	};

	SDL_RenderPresent(_tft->renderer);

	//if (_bpp == 16)
	//{
	//	bool oldSwapBytes = _tft->getSwapBytes();
	//	_tft->setSwapBytes(false);
	//	_tft->pushImage(x, y, _dwidth, _dheight, _img);
	//	_tft->setSwapBytes(oldSwapBytes);
	//}
	//else if (_bpp == 4)
	//{
	//	_tft->pushImage(x, y, _dwidth, _dheight, _img4, false, _colorMap);
	//}
	//else _tft->pushImage(x, y, _dwidth, _dheight, _img8, (bool)(_bpp == 8));
}

/***************************************************************************************
** Function name:           fillSprite
** Description:             Fill the whole sprite with defined colour
***************************************************************************************/
void TFT_eSprite::fillSprite(uint32_t color)
{
	if (!_created || _vpOoB) return;

	fillRect(_vpX - _xDatum, _vpY - _yDatum, _xWidth, _yHeight, color);
}

/***************************************************************************************
** Function name:           setColorDepth
** Description:             Set bits per pixel for colour (1, 8 or 16)
***************************************************************************************/
void* TFT_eSprite::setColorDepth(int8_t b)
{
	// Do not re-create the sprite if the colour depth does not change
	//if (_bpp == b) return _img8_1;

	// Validate the new colour depth
	if (b > 8) _bpp = 16;  // Bytes per pixel
	else if (b > 4) _bpp = 8;
	else if (b > 1) _bpp = 4;
	else _bpp = 1;

	// Can't change an existing sprite's colour depth so delete and create a new one
	if (_created) {
		deleteSprite();
		return createSprite(_dwidth, _dheight);
	}

	return nullptr;
}


/***************************************************************************************
** Function name:           getColorDepth
** Description:             Get bits per pixel for colour (1, 8 or 16)
***************************************************************************************/
int8_t TFT_eSprite::getColorDepth(void)
{
	if (_created) return _bpp;
	else return 0;
}

/***************************************************************************************
** Function name:           width
** Description:             Return the width of sprite
***************************************************************************************/
// Return the size of the sprite
int16_t TFT_eSprite::width(void)
{
	if (!_created) return 0;

	if (_bpp > 1) {
		if (_vpDatum) return _xWidth;
		return _dwidth;
	}

	if (rotation & 1) {
		if (_vpDatum) return _xWidth;
		return _dheight;
	}

	if (_vpDatum) return _xWidth;
	return _dwidth;
}


/***************************************************************************************
** Function name:           height
** Description:             Return the height of sprite
***************************************************************************************/
int16_t TFT_eSprite::height(void)
{
	if (!_created) return 0;

	if (_bpp > 1) {
		if (_vpDatum) return _yHeight;
		return _dheight;
	}

	if (rotation & 1) {
		if (_vpDatum) return _yHeight;
		return _dwidth;
	}

	if (_vpDatum) return _yHeight;
	return _dheight;
}

/***************************************************************************************
** Function name:           fillRect
** Description:             draw a filled rectangle
***************************************************************************************/
void TFT_eSprite::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
	if (!_created || _vpOoB) return;

	x += _xDatum;
	y += _yDatum;

	// Clipping
	if ((x >= _vpW) || (y >= _vpH)) return;

	if (x < _vpX) { w += x - _vpX; x = _vpX; }
	if (y < _vpY) { h += y - _vpY; y = _vpY; }

	if ((x + w) > _vpW) w = _vpW - x;
	if ((y + h) > _vpH) h = _vpH - y;

	if ((w < 1) || (h < 1)) return;
	 
	SDL_Rect dst{x, y, w, h};

	auto color32bpp = color16to24(color) | 0xFF000000;

	SDL_FillRect(_surface, &dst, color32bpp);
}