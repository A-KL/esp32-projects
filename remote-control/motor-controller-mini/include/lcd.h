#pragma once

#define LCD_SH1107 1
#define LCD_SSD1306 2

#define OLED_RESET -1

static bool _lcd_init = false;

#ifdef LCD_DRIVER

// #include <Fonts/FreeSans18pt7b.h>
#define FONT_NAME FreeSans12pt7b
#define FONT_FILE_NAME <Fonts/FONT_NAME.h>

#include FONT_FILE_NAME

#if (LCD_DRIVER == LCD_SH1107)
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SH110X.h>

  static Adafruit_SH1107 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

  bool driver_init()
  {
    _lcd_init = display.begin(LCD_ADDRESS, true); // Address 0x3D default

    if (!_lcd_init) { 
        log_w("LCD initialization...FAIL");     
        return false;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(QUOTE(DEVICE_ID));
    display.display();

    //display.drawRect(0, 0, SH110X_WHITE);
    return true;
  }
#endif

#if (LCD_DRIVER == LCD_SSD1306)
  #include <SPI.h>
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  static Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET);

  bool driver_init()
  {
      _lcd_init = display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS);

      if (!_lcd_init) { 
          log_w("LCD initialization...FAIL");     
          return false;
      }
      display.setFont(&FONT_NAME);


      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0,0);
      display.println(QUOTE(DEVICE_ID)); //display.getTextBounds(string, x, y, &x1, &y1, &w, &h);
      display.display();

      return true;
  }
#endif

#else
  inline bool driver_init() {
    return false;
  }
#endif



void lcd_init()
{
#if defined(I2C_SDA) && defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#endif
    if (driver_init())
    {
      log_i("LCD initialization...OK");
    }
}

void lcd_display(const int16_t* values, const uint8_t count) 
{
#ifdef LCD_DRIVER

#if (LCD_DRIVER==LCD_SSD1306)
  const uint8_t cols = 3;
  const uint8_t rows = 4;
#endif

#if (LCD_DRIVER==LCD_SH1107)
  const uint8_t cols = 3;
  const uint8_t rows = 6;
#endif

  const auto w_in = (LCD_WIDTH / cols);
  const auto h_in = (LCD_HEIGHT / rows);

  display.clearDisplay();
  for (uint8_t i = 0; i<cols; i++) {
    for (uint8_t j = 0; j<rows; j++) {
      display.setCursor(i * w_in, j * h_in);
      display.println(values[j+i*rows]);
    }
  }
  display.display();
#endif
}