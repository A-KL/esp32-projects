#pragma once

class TFT_eSprite : public TFT_eSPI {

public:

	explicit TFT_eSprite(TFT_eSPI* tft);
	~TFT_eSprite(void);

    // Create a sprite of width x height pixels, return a pointer to the RAM area
	// Sketch can cast returned value to (uint16_t*) for 16-bit depth if needed
	// RAM required is:
	//  - 1 bit per pixel for 1 bit colour depth
	//  - 1 nibble per pixel for 4-bit colour (with palette table)
	//  - 1 byte per pixel for 8-bit colour (332 RGB format)
	//  - 2 bytes per pixel for 16-bit color depth (565 RGB format)
    void* createSprite(int16_t width, int16_t height, uint8_t frames = 1);

	// Delete the sprite to free up the RAM
	void     deleteSprite(void);

	// Push the sprite to the TFT screen, this fn calls pushImage() in the TFT class.
	// Optionally a "transparent" colour can be defined, pixels of that colour will not be rendered
	void     pushSprite(int32_t x, int32_t y);
	//void     pushSprite(int32_t x, int32_t y, uint16_t transparent);

	// Set or get the colour depth to 1, 4, 8 or 16 bits. Can be used to change depth an existing
	// sprite, but clears it to black, returns a new pointer if sprite is re-created.
	void*	 setColorDepth(int8_t b);
	int8_t   getColorDepth(void);

	// Return the width and height of the sprite
	int16_t  width(void),
			 height(void);

	// Draw a single character in the GLCD or GFXFF font
	void
	//void     drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size),

			// Fill Sprite with a colour
			fillSprite(uint32_t color),

	//		// Define a window to push 16-bit colour pixels into in a raster order
	//		// Colours are converted to the set Sprite colour bit depth
	//		setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1),
	//		// Push a color (aka singe pixel) to the sprite's set window area
	//		pushColor(uint16_t color),
	//		// Push len colors (pixels) to the sprite's set window area
	//		pushColor(uint16_t color, uint32_t len),
	//		// Push a pixel pre-formatted as a 1, 4, 8 or 16-bit colour (avoids conversion overhead)
	//		writeColor(uint16_t color),

	//		// Set the scroll zone, top left corner at x,y with defined width and height
	//		// The colour (optional, black is default) is used to fill the gap after the scroll
	//		setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color = TFT_BLACK),
	//		// Scroll the defined zone dx,dy pixels. Negative values left,up, positive right,down
	//		// dy is optional (default is 0, so no up/down scroll).
	//		// The sprite coordinate frame does not move because pixels are moved
	//		scroll(int16_t dx, int16_t dy = 0),

	//		// Draw lines
	//		drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color),
	//		drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
	//		drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),

			// Fill a rectangular area with a color (aka draw a filled rectangle)
			fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

private:

	TFT_eSPI* _tft;

protected:

	uint8_t  _bpp;     // bits per pixel (1, 4, 8 or 16)\

	bool     _created; // A Sprite has been created and memory reserved
	bool     _gFont = false;

	int32_t  _xs, _ys, _xe, _ye, _xptr, _yptr; // for setWindow
	int32_t  _sx, _sy; // x,y for scroll zone
	uint32_t _sw, _sh; // w,h for scroll zone
	uint32_t _scolor;  // gap fill colour for scroll zone

	int32_t  _iwidth, _iheight; // Sprite memory image bit width and height (swapped during rotations)
	int32_t  _dwidth, _dheight; // Real sprite width and height (for <8bpp Sprites)
	int32_t  _bitwidth;         // Sprite image bit width for drawPixel (for <8bpp Sprites, not swapped)

	SDL_Surface* _surface;
};