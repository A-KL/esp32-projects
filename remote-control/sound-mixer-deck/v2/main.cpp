#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Fonts/FreeSansBold18pt7b.h> // A custom font

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  Serial.begin(115200);

  tft.init(76, 284); // Initialize display with custom dimensions and SPI mode

  tft.setFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.setTextWrap(false);

  tft.fillScreen(ST77XX_RED);
  tft.setCursor(0, 0);
  tft.println("Hello, World!");
}

void loop() {
}