#pragma once

#ifdef DISPLAY_ENABLED

#include <ArduinoLog.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define SCREEN_WIDTH SH1106_LCDHEIGHT  // OLED display width, in pixels
#define SCREEN_HEIGHT SH1106_LCDWIDTH // OLED display height, in pixels

Adafruit_SH1106 display(-1);

static unsigned short display_rows[3];

// void display_channel(unsigned short index, int value)
// {
//   display.clearDisplay();
//   //display.drawRect(0, 0, 10, 10, WHITE);
//   display.setTextSize(2);
//   display.setTextColor(WHITE);     // text color
//  // display.setCursor(5, 8);        // position to display
  
//   for (auto &row : display_rows) {
//     display.println(row);
//     Log.warningln(row);
//    // display.setCursor(5, 8 * ());
//   }

//   display.display();
// }

#endif

void display_init()
{
#ifdef DISPLAY_ENABLED
    display.begin(SH1106_SWITCHCAPVCC, SH1106_I2C_ADDRESS, false);
    display.setRotation(1);
    display.clearDisplay();
#endif
}

static unsigned long last_updated = millis();

void display_progress_bar(int top, const unsigned short value, const unsigned short min = 0, unsigned short max = (ADC_MAX - 1))
{
    auto max_w = SCREEN_WIDTH - 1 - 2 * 2;
    auto actual_w = map(value, min, max, 0, max_w);

    //Log.warningln("disp: %d", actual_w);

    display.drawRect(2, top, max_w, 10, WHITE);
    display.fillRect(2, top, actual_w, 10, WHITE);
}

void display_update()
{
#ifdef DISPLAY_ENABLED
    auto now = millis();
    if ((now - last_updated) < 150) {   
        return;
    }
    last_updated = now;

    display.clearDisplay();
    //display.drawRect(0, 0, 10, 10, WHITE);
    // display.setTextSize(1);
    // display.setTextColor(WHITE);
    // display.setCursor(0,0);

    display_progress_bar(2, display_rows[0]);

    display_progress_bar(14, display_rows[1]);

    display_progress_bar(26, display_rows[2]);

    // auto max_w = SCREEN_WIDTH - 1 - 2 * 2;
    // auto actual_w = map(display_rows[0], 0, (ADC_MAX - 1), 0, max_w);

    // //Log.warningln("disp: %d", actual_w);

    // display.drawRect(2, 2, max_w, 10, WHITE);
    // display.fillRect(2, 2, actual_w, 10, WHITE);

    //display.println(display_rows[0]);
    // for (auto &row : display_rows) {
    //   display.println(row);
    //   Log.warningln("disp: %d", row);
    // }
    display.display();
#endif
}