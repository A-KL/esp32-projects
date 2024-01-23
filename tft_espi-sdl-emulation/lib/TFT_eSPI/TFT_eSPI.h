/***************************************************
  Arduino TFT graphics library targeted at ESP8266
  and ESP32 based boards.

  This is a stand-alone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The built-in fonts 4, 6, 7 and 8 are Run Length
  Encoded (RLE) to reduce the FLASH footprint.

  Last review/edit by Bodmer: 04/02/22
 ****************************************************/

// Stop fonts etc. being loaded multiple times
#ifndef _TFT_eSPIH_
#define _TFT_eSPIH_

#define TFT_ESPI_VERSION "2.5.42"

//#include <User_Setup_Select.h>

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/
#ifndef TAB_COLOUR
  #define TAB_COLOUR 0
#endif

/***************************************************************************************
**                         Section 4: Setup fonts
***************************************************************************************/
// Use GLCD font in error case where user requests a smooth font file
// that does not exist (this is a temporary fix to stop ESP32 reboot)
#ifdef SMOOTH_FONT
  #ifndef LOAD_GLCD
    #define LOAD_GLCD
  #endif
#endif

// Only load the fonts defined in User_Setup.h (to save space)
// Set flag so RLE rendering code is optionally compiled
#ifdef LOAD_GLCD
  #include <Fonts/glcdfont.c>
#endif

#ifdef LOAD_FONT2
  #include <Fonts/Font16.h>
#endif

#ifdef LOAD_FONT4
  #include <Fonts/Font32rle.h>
  #define LOAD_RLE
#endif

#ifdef LOAD_FONT6
  #include <Fonts/Font64rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT7
  #include <Fonts/Font7srle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT8
  #include <Fonts/Font72rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#elif defined LOAD_FONT8N // Optional narrower version
  #define LOAD_FONT8
  #include <Fonts/Font72x53rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#include <SDL2/SDL.h>

/***************************************************************************************
**                         Section 5: Font datum enumeration
***************************************************************************************/
//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

// Next is a special 16-bit colour value that encodes to 8 bits
// and will then decode back to the same 16-bit value.
// Convenient for 8-bit and 16-bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

/**************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/
// Callback prototype for smooth font pixel colour read
typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

// Class functions and variables
class TFT_eSPI { friend class TFT_eSprite; // Sprite class has access to protected members

 //--------------------------------------- public ------------------------------------//
 public:

  TFT_eSPI(int16_t _W = TFT_WIDTH, int16_t _H = TFT_HEIGHT);

  // init() and begin() are equivalent, begin() included for backwards compatibility
  // Sketch defined tab colour option is for ST7735 displays only
  void     init(uint8_t tc = TAB_COLOUR), begin(uint8_t tc = TAB_COLOUR);

  void     destroy();

  // These are virtual so the TFT_eSprite class can override them with sprite specific functions
  virtual void      drawPixel(int32_t x, int32_t y, uint32_t color),
  //                  drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size),
                    drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color),
                    drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
                    drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
                    fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

  virtual int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
                   drawChar(uint16_t uniCode, int32_t x, int32_t y),
                   height(void),
                   width(void);

                   // Read the colour of a pixel at x,y and return value in 565 format
  virtual uint16_t readPixel(int32_t x, int32_t y);

  // virtual void     setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);   // Note: start + end coordinates

                   // Push (aka write pixel) colours to the set window
  //virtual void     pushColor(uint16_t color);

 void     setRotation(uint8_t r); // Set the display image orientation to 0, 1, 2 or 3
 uint8_t  getRotation(void);      // Read the current rotation

  // Change the origin position from the default top left
  // Note: setRotation, setViewport and resetViewport will revert origin to top left corner of screen/sprite
  void     setOrigin(int32_t x, int32_t y);
  int32_t  getOriginX(void);
  int32_t  getOriginY(void);

//   void     invertDisplay(bool i);  // Tell TFT to invert all displayed colours


  // The TFT_eSprite class inherits the following functions (not all are useful to Sprite class
  void     setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h); // Note: start coordinates + width and height

  // Viewport commands, see "Viewport_Demo" sketch
  void     setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum = true);
  bool     checkViewport(int32_t x, int32_t y, int32_t w, int32_t h);
  int32_t  getViewportX(void);
  int32_t  getViewportY(void);
  int32_t  getViewportWidth(void);
  int32_t  getViewportHeight(void);
  bool     getViewportDatum(void);
  void     frameViewport(uint16_t color, int32_t w);
  void     resetViewport(void);

           // Clip input window to viewport bounds, return false if whole area is out of bounds
  bool     clipAddrWindow(int32_t* x, int32_t* y, int32_t* w, int32_t* h);
           // Clip input window area to viewport bounds, return false if whole area is out of bounds
  bool     clipWindow(int32_t* xs, int32_t* ys, int32_t* xe, int32_t* ye);

//            // Push (aka write pixel) colours to the TFT (use setAddrWindow() first)
//   void     pushColor(uint16_t color, uint32_t len),  // Deprecated, use pushBlock()
//            pushColors(uint16_t  *data, uint32_t len, bool swap = true), // With byte swap option
//            pushColors(uint8_t  *data, uint32_t len); // Deprecated, use pushPixels()

//            // Write a solid block of a single colour
//   void     pushBlock(uint16_t color, uint32_t len);

//            // Write a set of pixels stored in memory, use setSwapBytes(true/false) function to correct endianess
//   void     pushPixels(const void * data_in, uint32_t len);

//            // Support for half duplex (bi-directional SDA) SPI bus where MOSI must be switched to input
//            #ifdef TFT_SDA_READ
//              #if defined (TFT_eSPI_ENABLE_8_BIT_READ)
//   uint8_t  tft_Read_8(void);     // Read 8-bit value from TFT command register
//              #endif
//   void     begin_SDA_Read(void); // Begin a read on a half duplex (bi-directional SDA) SPI bus - sets MOSI to input
//   void     end_SDA_Read(void);   // Restore MOSI to output
//            #endif


  // Graphics drawing
  void     fillScreen(uint32_t color);
//            drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color),
//            drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color),
//            fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);

  void     fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);
  void     fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2);

   void     
           drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color),
           fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color),

           drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
           fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),

           //                 Corner 1               Corner 2               Corner 3
           drawTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color),
           fillTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color);


  // Smooth (anti-aliased) graphics drawing
           // Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
           // If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
 uint16_t   drawPixel(int32_t x, int32_t y, uint32_t color, uint8_t alpha, uint32_t bg_color = 0x00FFFFFF);

//            // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
//            // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
//            // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
//            // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
//   void     drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool roundEnds = false);

//            // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
//            // arc segments and ensures clean segment joints. 
//            // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
//   void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color, uint32_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);
  
           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color, uint32_t bg_color = 0x00FFFFFF);

//            // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
//            // If bg_color is not included the background pixel colour will be read from TFT or sprite
//   void     drawSpot(float ax, float ay, float r, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

//            // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
//            // If bg_color is not included the background pixel colour will be read from TFT or sprite
//   void     drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);

//            // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
//            // If bg_color is not included the background pixel colour will be read from TFT or sprite
//   void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);


  // Image rendering
           // Swap the byte order for pushImage() and pushPixels() - corrects endianness
  void     setSwapBytes(bool swap);
  bool     getSwapBytes(void);

//            // Draw bitmap
//   void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
//            drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
//            drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor),
//            drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t fgcolor, uint16_t bgcolor),
//            setBitmapColor(uint16_t fgcolor, uint16_t bgcolor); // Define the 2 colours for 1bpp sprites

//            // Set TFT pivot point (use when rendering rotated sprites)
//   void     setPivot(int16_t x, int16_t y);
//   int16_t  getPivotX(void), // Get pivot x
//            getPivotY(void); // Get pivot y

//            // The next functions can be used as a pair to copy screen blocks (or horizontal/vertical lines) to another location
//            // Read a block of pixels to a data buffer, buffer is 16-bit and the size must be at least w * h
//   void     readRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
//            // Write a block of pixels to the screen which have been read by readRect()
//   void     pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

//            // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transparent);

//            // These are used to render images stored in FLASH (PROGMEM)
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

//            // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
//            // They are not intended to be used with user sketches (but could be)
//            // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);
//            // FLASH version
//   void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr);

//            // Render a 16-bit colour image with a 1bpp mask
//   void     pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask);

//            // This next function has been used successfully to dump the TFT screen to a PC for documentation purposes
//            // It reads a screen area and returns the 3 RGB 8-bit colour values of each pixel in the buffer
//            // Set w and h to 1 to read 1 pixel's colour. The data buffer must be at least w * h * 3 bytes
//   void     readRectRGB(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data);


//   // Text rendering - value returned is the pixel width of the rendered text
//   int16_t  drawNumber(long intNumber, int32_t x, int32_t y, uint8_t font), // Draw integer using specified font number
//            drawNumber(long intNumber, int32_t x, int32_t y),               // Draw integer using current font

//            // Decimal is the number of decimal places to render
//            // Use with setTextDatum() to position values on TFT, and setTextPadding() to blank old displayed values
//            drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y, uint8_t font), // Draw float using specified font number
//            drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y)               // Draw float using current font

//            // Handle char arrays
//            // Use with setTextDatum() to position string on TFT, and setTextPadding() to blank old displayed strings
//         //    drawString(const char *string, int32_t x, int32_t y, uint8_t font),  // Draw string using specified font number
//         //    drawString(const char *string, int32_t x, int32_t y),                // Draw string using current font
//         //    drawString(const String& string, int32_t x, int32_t y, uint8_t font),// Draw string using specified font number
//         //    drawString(const String& string, int32_t x, int32_t y),              // Draw string using current font

//         //    drawCentreString(const char *string, int32_t x, int32_t y, uint8_t font),  // Deprecated, use setTextDatum() and drawString()
//         //    drawRightString(const char *string, int32_t x, int32_t y, uint8_t font),   // Deprecated, use setTextDatum() and drawString()
//         //    drawCentreString(const String& string, int32_t x, int32_t y, uint8_t font),// Deprecated, use setTextDatum() and drawString()
//         //    drawRightString(const String& string, int32_t x, int32_t y, uint8_t font); // Deprecated, use setTextDatum() and drawString()
//         ;


//   // Text rendering and font handling support functions
//   void     setCursor(int16_t x, int16_t y),                 // Set cursor for tft.print()
//            setCursor(int16_t x, int16_t y, uint8_t font);   // Set cursor and font number for tft.print()

//   int16_t  getCursorX(void),                                // Read current cursor x position (moves with tft.print())
//            getCursorY(void);                                // Read current cursor y position

//   void     setTextColor(uint16_t color),                    // Set character (glyph) color only (background not over-written)
//            setTextColor(uint16_t fgcolor, uint16_t bgcolor, bool bgfill = false),  // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
//            setTextSize(uint8_t size);                       // Set character size multiplier (this increases pixel size)

//   void     setTextWrap(bool wrapX, bool wrapY = false);     // Turn on/off wrapping of text in TFT width and/or height

//   void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 5 above
//   uint8_t  getTextDatum(void);

//   void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking/over-write) width in pixels
//   uint16_t getTextPadding(void);                            // Get text padding

// #ifdef LOAD_GFXFF
//   void     setFreeFont(const GFXfont *f = NULL),            // Select the GFX Free Font
//            setTextFont(uint8_t font);                       // Set the font number to use in future
// #else
//   void     setFreeFont(uint8_t font),                       // Not used, historical fix to prevent an error
//            setTextFont(uint8_t font);                       // Set the font number to use in future
// #endif

int16_t
//   int16_t  textWidth(const char *string, uint8_t font),     // Returns pixel width of string in specified font
//            textWidth(const char *string),                   // Returns pixel width of string in current font
//         //    textWidth(const String& string, uint8_t font),   // As above for String types
//         //    textWidth(const String& string),
           fontHeight(int16_t font),                        // Returns pixel height of specified font
           fontHeight(void);                                // Returns pixel height of current font

//            // Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
//   uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining),
//            decodeUTF8(uint8_t c);

//            // Support function to UTF8 decode and draw characters piped through print stream
//   size_t   write(uint8_t);
//            // size_t   write(const uint8_t *buf, size_t len);

  // Used by Smooth font class to fetch a pixel colour for the anti-aliasing
  void     setCallback(getColorCallback getCol);

//   uint16_t fontsLoaded(void); // Each bit in returned value represents a font type that is loaded - used for debug/error handling only


  // Colour conversion
           // Convert 8-bit red, green and blue to 16 bits
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Convert 8-bit colour to 16 bits
  uint16_t color8to16(uint8_t color332);
           // Convert 16-bit colour to 8 bits
  uint8_t  color16to8(uint16_t color565);

           // Convert 16-bit colour to/from 24-bit, R+G+B concatenated into LS 24 bits
  uint32_t color16to24(uint16_t color565);
  uint32_t color24to16(uint32_t color888);

  //          // Alpha blend 2 colours, see generic "alphaBlend_Test" example
  //          // alpha =   0 = 100% background colour
  //          // alpha = 255 = 100% foreground colour
  // uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

  //          // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
  // uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
  //          // 24-bit colour alphaBlend with optional alpha dither
  // uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);

  uint32_t textcolor, textbgcolor;         // Text foreground and background colours

  uint32_t bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

  uint8_t  textfont,  // Current selected font number
           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

 private:
  getColorCallback getColor = nullptr; // Smooth font callback function pointer

  bool     locked, inTransaction, lockTransaction; // SPI transaction and mutex lock flags

            // Smooth graphics helper
  uint8_t  sqrt_fraction(uint32_t num);

            // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

 protected:
  void setRenderColor(uint32_t color);
  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  int32_t  _init_width, _init_height; // Display w/h as input, used by setRotation()
  int32_t  _width, _height;           // Display w/h as modified by current rotation
  int32_t  addr_row, addr_col;        // Window position - used to minimise window commands

  int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
  int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

  // Viewport variables
  int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
  int32_t  _xDatum;
  int32_t  _yDatum;
  int32_t  _xWidth;
  int32_t  _yHeight;
  bool     _vpDatum;
  bool     _vpOoB;

  int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting
  int32_t  bg_cursor_x;                    // Background fill cursor
  int32_t  last_cursor_x;                  // Previous text cursor position when fill used

  uint32_t fontsloaded;               // Bit field of fonts loaded

  uint8_t  glyph_ab,   // Smooth font glyph delta Y (height) above baseline
           glyph_bb;   // Smooth font glyph delta Y (height) below baseline

  bool     isDigits;   // adjust bounding box for numbers to reduce visual jiggling
  bool     textwrapX, textwrapY;  // If set, 'wrap' text at right and optionally bottom edge of display
  bool     _swapBytes; // Swap the byte order for TFT pushImage()

  bool     _booted;    // init() or begin() has already run once

                       // User sketch manages these via set/getAttribute()
  bool     _cp437;        // If set, use correct CP437 charset (default is OFF)
  bool     _utf8;         // If set, use UTF-8 decoder in print stream 'write()' function (default ON)
  bool     _psram_enable; // Enable PSRAM use for library functions (TBD) and Sprites

  uint32_t _lastColor; // Buffered value of last colour used

  bool     _fillbg;    // Fill background flag (just for for smooth fonts at the moment)

public:
  // Get the screen touch coordinates, returns true if screen has been touched
  // if the touch coordinates are off screen then x and y are not updated
  // The returned value can be treated as a bool type, false or 0 means touch not detected
  // In future the function may return an 8-bit "quality" (jitter) value.
  // The threshold value is optional, this must be higher than the bias level for z (pressure)
  // reported by Test_Touch_Controller when the screen is NOT touched. When touched the z value
  // must be higher than the threshold for a touch to be detected.
  uint8_t  getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);

}; // End of class TFT_eSPI

// Swap any type
template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }

// Fast alphaBlend
template <typename A, typename F, typename B> static inline uint16_t
fastBlend(A alpha, F fgc, B bgc)
{
  // Split out and blend 5-bit red and blue channels
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  // Split out and blend 6-bit green channel
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  // Recombine channels
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}

/***************************************************************************************
**                         Section 10: Additional extension classes
***************************************************************************************/
// Load the Button Class
//#include "Extensions/Button.h"

// Load the Sprite Class
//#include "Extensions/Sprite.h"

#endif // ends #ifndef _TFT_eSPIH_