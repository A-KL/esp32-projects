#include "Final_Frontier_28.h"

#include "NotoSans_Bold.h"
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"

#include "NotoSansMonoSCB20.h"

// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36
#define AA_FONT_MONO  NotoSansMonoSCB20 // NotoSansMono-SemiCondensedBold 20pt

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library
TFT_eSprite spr = TFT_eSprite(&tft); // Sprite class needs to be invoked

void button(int x, int y, int num );

void setup() {
  Serial.begin(115200); // Used for messages

  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.fillScreen(TFT_YELLOW);

  spr.setColorDepth(16); // 16 bit colour needed to show antialiased fonts
}

void draw_tile(TFT_eSprite& spr, int32_t w, int32_t h, int32_t color_title, int32_t color_tile, const char * title)
{
  int32_t radius = 15;

  int32_t x = 0;
  int32_t y = 0;

  int32_t title_padding = radius / 2;

  spr.setTextColor(TFT_BLACK, color_title);

  spr.fillSmoothCircle(x + radius, y + radius, radius, color_title);

  spr.fillSmoothCircle(x + w - radius - 1, y + radius, radius, color_title);

  spr.fillRect(x + radius, y, w - radius * 2, radius, color_title);

  spr.fillRect(x, y + radius, w, title_padding, color_title);

  spr.drawString(title, radius, 4);

  spr.fillRect(x, y + radius + title_padding, w, h - radius * 2 - title_padding, color_tile);

  spr.fillSmoothCircle(x + radius, y + h - radius, radius, color_tile);

  spr.fillSmoothCircle(x + w - radius - 1, y + h - radius, radius, color_tile);

  spr.fillRect(x + radius, y + h - radius, w - radius * 2, radius, color_tile);
}

void draw_ui(TFT_eSprite& spr, int32_t x, int32_t y, int32_t w, int32_t h, int32_t color_title, int32_t color_tile, const char * title)
{
  spr.createSprite(w, h);

  draw_tile(spr, w, h, color_title, color_tile, title);

  spr.pushSprite(x, y);
  
  spr.deleteSprite();
}

void draw_gradient(TFT_eSprite& spr, int32_t x, int32_t y, int32_t w, int32_t h)
{
  spr.createSprite(w, h);

  for(auto i=0; i<255; i++)
  {
    spr.fillRect(i, 0, 2, 239, tft.color565(i,0,0));
  }

  spr.fillRect(10, 10, 4, 239, tft.color565(100,0,0));

  spr.pushSprite(0, 0);
  
  spr.deleteSprite();
}

void loop() {
  spr.loadFont(AA_FONT_SMALL);

  tft.fillScreen(TFT_BLACK);

  auto margin = 5;
  auto width = 100;
  auto height = 110;

  //draw_gradient(spr, 0, 0, 255, 240);

  draw_ui(spr, margin, margin, width, height, TFT_GREEN, TFT_DARKGREEN, "Motors");

  draw_ui(spr, margin, height + margin * 2, width, height, TFT_RED, TFT_MAROON, "Power");


  draw_ui(spr, width + margin * 2, margin, width, height, TFT_ORANGE, tft.color565(205, 145, 0), "Xbox");

  draw_ui(spr, width + margin * 2, height + margin * 2, width, height, tft.color565(125, 145, 100), tft.color565(50, 50, 50), "Encoder");


  draw_ui(spr, width * 2 + margin * 3, margin, width, height, TFT_MAGENTA, tft.color565(185, 0, 185), "Motors");

  draw_ui(spr, width * 2 + margin * 3, height + margin * 2, width, height, TFT_DARKGREY, tft.color565(70, 70, 70), "Power");

  delay(4000);

  return;

  int xpos = tft.width() / 2; // Half the screen width
  int ypos = 50;


  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Small font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  spr.loadFont(AA_FONT_SMALL); // Must load the font first into the sprite class

  spr.createSprite(100, 50);   // Create a sprite 100 pixels wide and 50 high

  spr.fillSprite(TFT_BLUE);

  spr.drawRect(0, 0, 100, 50, TFT_WHITE); // Draw sprite border outline (so we see extent)

  spr.setTextColor(TFT_YELLOW, TFT_DARKGREY); // Set the sprite font colour and the background colour

  spr.setTextDatum(MC_DATUM); // Middle Centre datum
  
  spr.drawString("15pt font", 50, 25 ); // Coords of middle of 100 x 50 Sprite

  spr.pushSprite(10, 10); // Push to TFT screen coord 10, 10

  spr.pushSprite(10, 70, TFT_BLUE); // Push to TFT screen, TFT_BLUE is transparent
 
  spr.unloadFont(); // Remove the font from sprite class to recover memory used

  delay(4000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Large font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  tft.fillScreen(TFT_BLACK);

  // Beware: Sprites are a differerent "class" to TFT, so different fonts can be loaded
  // in the tft and sprite instances, so load the font in the class instance you use!
  // In this example this means the spr. instance.

  spr.loadFont(AA_FONT_LARGE); // Load another different font into the sprite instance

  // 100 x 50 sprite was created above and still exists...

  spr.fillSprite(TFT_GREEN);

  spr.setTextColor(TFT_BLACK, TFT_GREEN); // Set the font colour and the background colour

  spr.setTextDatum(MC_DATUM); // Middle Centre datum

  spr.drawString("Fits", 50, 25); // Make sure text fits in the Sprite!
  spr.pushSprite(10, 10);         // Push to TFT screen coord 10, 10

  spr.fillSprite(TFT_RED);
  spr.setTextColor(TFT_WHITE, TFT_RED); // Set the font colour and the background colour

  spr.drawString("Too big", 50, 25); // Text is too big to all fit in the Sprite!
  spr.pushSprite(10, 70);            // Push to TFT screen coord 10, 70

  // Draw changing numbers - no flicker using this plot method!

  // >>>> Note: it is best to use drawNumber() and drawFloat() for numeric values <<<<
  // >>>> this reduces digit position movement when the value changes             <<<<
  // >>>> drawNumber() and drawFloat() functions behave like drawString() and are <<<<
  // >>>> supported by setTextDatum() and setTextPadding()                        <<<<

  spr.setTextDatum(TC_DATUM); // Top Centre datum

  spr.setTextColor(TFT_WHITE, TFT_BLUE); // Set the font colour and the background colour

  for (int i = 0; i <= 200; i++) {
    spr.fillSprite(TFT_BLUE);
    spr.drawFloat(i / 100.0, 2, 50, 10); // draw with 2 decimal places at 50,10 in sprite
    spr.pushSprite(10, 130); // Push to TFT screen coord 10, 130
    delay (20);
  }

  spr.unloadFont(); // Remove the font to recover memory used

  spr.deleteSprite(); // Recover memory
  
  delay(1000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Mono spaced font
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  
  spr.loadFont(AA_FONT_MONO); // Mono spaced fonts have fixed inter-character gaps to
                              // aid formatting
  int bnum = 1;

  // Example of drawing buttons
  for (int j = 0; j < 4; j++)
  {
    for (int k = 0; k < 4; k++)
    {
      int x = 120 + k * 45;
      int y = 40  + j * 30;
      button(x, y, bnum++);
    }
  }

  for (int i = 0; i < 100; i++)
  {
    button(120, 160, i);
    delay(50);
  }
  
  spr.unloadFont();

  delay(8000);
}


// #########################################################################
// Draw a number in a rounded rectangle with some transparent pixels
// Load the font before calling
// #########################################################################
void button(int x, int y, int num )
{

  // Size of sprite
  #define IWIDTH  40
  #define IHEIGHT 25

  // Create a 16 bit sprite 40 pixels wide, 25 high (2000 bytes of RAM needed)
  spr.setColorDepth(16);
  spr.createSprite(IWIDTH, IHEIGHT);

  // Fill it with black (this will be the transparent colour this time)
  spr.fillSprite(TFT_BLACK);

  // Draw a background for the numbers
  spr.fillRoundRect(  0, 0,  IWIDTH, IHEIGHT, 8, TFT_RED);
  spr.drawRoundRect(  0, 0,  IWIDTH, IHEIGHT, 8, TFT_WHITE);

  // Set the font parameters

  // Set text coordinate datum to middle centre
  spr.setTextDatum(MC_DATUM);

  // Set the font colour and the background colour
  spr.setTextColor(TFT_WHITE, TFT_RED);

  // Draw the number
  spr.drawNumber(num, IWIDTH/2, 1 + IHEIGHT/2);

  // Push sprite to TFT screen CGRAM at coordinate x,y (top left corner)
  // All black pixels will not be drawn hence will show as "transparent"
  spr.pushSprite(x, y, TFT_BLACK);

  // Delete sprite to free up the RAM
  spr.deleteSprite();
}