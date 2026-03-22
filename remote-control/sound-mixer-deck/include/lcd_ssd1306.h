#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void display_percentage(const char* label, uint8_t perc)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds("%", 0, 0, &x1, &y1, &w, &h);

  display.clearDisplay();
  display.setTextSize(2);
  
  display.fillRect(0,0, SCREEN_WIDTH/2 - 5, SCREEN_HEIGHT, WHITE);
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

  display.setTextColor(BLACK);
  display.setCursor(5, 8);
  display.print(label);

  display.setTextColor(WHITE);

  auto chars_count = perc < 10 ? 1 : perc < 100 ? 2 : 3;

  display.setCursor(SCREEN_WIDTH - (w * (chars_count + 1)) - 10, 8);
  display.print(perc);
  display.println("%");

  display.display();
}

void display_text(const char* label)
{
  display.clearDisplay();
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);     // text color
  display.setCursor(5, 8);        // position to display
  display.println(label);
  display.display();
}
