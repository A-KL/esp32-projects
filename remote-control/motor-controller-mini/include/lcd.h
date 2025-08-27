#pragma once

#define OLED_RESET -1

#ifdef LCD_SH1107
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SH110X.h>

    static Adafruit_SH1107 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

#elif LCD_SSD1306
    #include <SPI.h>
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>

    static Adafruit_SSD1306 display(LCD_WIDTH, LCD_HEIGHT, &Wire, OLED_RESET);
#endif

static bool _lcd_init = false;

void sh110x_init()
{
#ifdef LCD_SH1107
    _lcd_init = display.begin(LCD_ADDRESS, true); // Address 0x3D default

    if (!_lcd_init) { 
        log_w("LCD initialization...FAIL");     
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(QUOTE(DEVICE_ID));
    display.display();

    //display.drawRect(0, 0, SH110X_WHITE);
#endif
}

void ssd1306_init()
{
#ifdef LCD_SSD1306
    _lcd_init = display.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS);

    if (!_lcd_init) { 
        log_w("LCD initialization...FAIL");     
        return;
    }

    display.clearDisplay();
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

void lcd_display(const int16_t* values, const uint8_t count) {
#ifdef LCD_SSD1306
  const uint8_t cols = 2;
  const uint8_t rows = 3;
#elif LCD_SH1107
  const uint8_t cols = 3;
  const uint8_t rows = 5;
#endif

  const auto w_in = (LCD_WIDTH / cols);
  const auto h_in = (LCD_HEIGHT / rows);

  for (uint8_t i = 0; i<cols; i++) {
    for (uint8_t j = 0; j<rows; j++) {
      display.setCursor(i * w_in, j * h_in);
      display.println(values[i+j]);
    }
  }
}

void scan_i2c(){
  Wire.begin(I2C_SDA, I2C_SCL);

  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}