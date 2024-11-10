#pragma once

#ifdef HAS_LCD
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

inline void lcd_init()
{
#ifdef I2C_SDA && I2C_SCL
    Wire.begin(I2C_SDA, I2C_SCL);
#endif

#ifdef HAS_LCD
    static Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, -1);

    if (!display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
        log_w("LCD initialization...FAIL");
        return;
    }

    display.clearDisplay();
    //display.setTextSize(1);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,28);
    display.println(HOSTNAME);
    display.display();

    //display.setCursor(0,28);

    log_i("LCD initialization...OK");
#endif
}