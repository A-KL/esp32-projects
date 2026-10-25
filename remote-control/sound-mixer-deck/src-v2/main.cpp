#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#include "parsing.h"

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

#define SCREEN_WIDTH 284
#define SCREEN_HEIGHT 76

Adafruit_ST7789 display(TFT_CS, TFT_DC, TFT_RST);

static long long last_received = millis();
static bool no_signal = false;

void draw_tile(const char* main_text, const char* sub_text, uint16_t color, int x, int y, int width = 70, int height = 76)
{
  display.fillRect(x+2, y+2, width-3*2, height-3*2, ST77XX_BLACK);
  display.flush();

  // FPS label
  display.setTextColor(color);
  display.setFont(&FreeSans9pt7b);
  display.setCursor(x + 15, 12);
  display.print("FPS");

  // FPS wattage
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(x + 15, 70);
  display.print(sub_text);
  
  // FPS value
  display.setTextColor(color);
  display.setFont(&FreeSansBold18pt7b);
  display.setCursor(x + 15, 50);
  display.print(main_text);
}

void setup() {
  Serial.begin(115200);

  display.init(SCREEN_HEIGHT, SCREEN_WIDTH); // Initialize display with custom dimensions and SPI mode
  display.invertDisplay(false);
  display.setRotation(1);
  // display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(ST77XX_WHITE);

  display.fillScreen(ST77XX_BLACK);

  auto x = 0;

  display.drawRect(x      , 0, 70, 76, ST77XX_YELLOW);
  display.drawRect(x+1    , 1, 68, 74, ST77XX_YELLOW);

  display.drawRect(x += 71, 0, 70, 76, ST77XX_RED);
  display.drawRect(x += 71, 0, 70, 76, ST77XX_BLUE);
  display.drawRect(x += 71, 0, 70, 76, ST77XX_MAGENTA);

  // FPS label
  display.setTextColor(ST77XX_YELLOW);
  display.setCursor(15, 12);
  display.setFont(&FreeSans9pt7b);
  display.println("FPS");

  // FPS wattage
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(15, 70);
  display.println("150w");
  
  // FPS value
  display.setTextColor(ST77XX_YELLOW);
  display.setCursor(15, 50);
  display.setFont(&FreeSansBold18pt7b);
  display.println("60");
}

void loop() {
   if (Serial.available()) {
    auto command = Serial.readStringUntil('\r');
    
    // display.setFont();
    // display.setTextSize(1);
    // display.setTextColor(ST77XX_WHITE);
    // display.setCursor(0, 2);
    // display.fillScreen(ST77XX_BLACK);

    String id;
    String property;
    String value;

    // CPU
    command = parse_input(command, id, property, value);
    // display.print(id);
    // display.print(": ");
    // display.print(value);
    // display.print(" | ");

    command = parse_input(command, id, property, value);
    // display.println(value);

    // RAM
    command = parse_input(command, id, property, value);
    // display.print(id);
    // display.print(": ");
    // display.println(value);

    // GPU
    command = parse_input(command, id, property, value);
    // display.print(id);
    // display.print(": ");
    // display.print(value);
    // display.print(" | ");

    draw_tile(value.c_str(), "150w", ST77XX_RED, 0, 0);

    // command = parse_input(command, id, property, value);
    // display.println(value);

    //display.flush();

    last_received = millis();
    no_signal = false;
  }

  if ((millis() - last_received) > 1000 && !no_signal) {
    no_signal = true;
   // display.fillScreen(ST77XX_BLACK);
    display.setFont(&FreeSansBold18pt7b);
    display.setCursor(0, 76);
    //display.println("NO SIGNAL");
  }
}