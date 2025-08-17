#pragma once

#ifdef HAS_LCD
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>
#endif

void lcd_init()
{
#if defined(I2C_SDA) && defined(I2C_SCL)
    Wire.begin(I2C_SDA, I2C_SCL);
#endif

#ifdef HAS_LCD
    static Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, -1);

    if (!display.begin(SSD1306_EXTERNALVCC, LCD_ADDRESS)) { //SSD1306_SWITCHCAPVCC
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

    //display.setCursor(0,28);

    log_i("LCD initialization...OK");
#endif
}