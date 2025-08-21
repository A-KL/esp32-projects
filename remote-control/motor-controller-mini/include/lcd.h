#pragma once

#define OLED_RESET -1

#ifdef LCD_SH1107
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SH110X.h>
#elif LCD_SSD1306
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

static bool _lcd_init = false;

void sh110x_init()
{
#ifdef LCD_SH1107
    static Adafruit_SH1107 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

    display.begin(LCD_ADDRESS, true); // Address 0x3D default
    display.clearDisplay();
    display.display();

    display.drawPixel(10, 10, SH110X_WHITE);
#endif
}

void ssd1306_init()
{
#ifdef LCD_SSD1306
    static Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET);

    _lcd_init = display.begin(SSD1306_EXTERNALVCC, LCD_ADDRESS); //SSD1306_SWITCHCAPVCC

    if (_lcd_init) { 
        log_w("LCD initialization...FAIL");     
        return;
    }

    display.clearDisplay();
    //display.setTextSize(1);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(QUOTE(DEVICE_ID));
    display.display();

    log_i("LCD initialization...OK");
#endif
}

void lcd_init()
{
#if defined(I2C_SDA) && defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#endif
    ssd1306_init();
    sh110x_init();
}