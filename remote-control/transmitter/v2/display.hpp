#pragma once

#ifdef DISPLAY_ENABLED

#include <ArduinoLog.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH        64
#define SCREEN_HEIGHT       128
#define SCREEN_RESET_PIN    -1
#define SCREEN_ADDRESS      0x3C

#define WHITE SH110X_WHITE
#define BLACK SH110X_BLACK

Adafruit_SH1107 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN, 400000, 100000);

static unsigned short display_rows[8] = {0};

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
    if(!display.begin(SCREEN_ADDRESS, true)) {
        Log.errorln("Failed to initialize display");
        return;
    }
    display.setRotation(0);
    display.clearDisplay();

    Log.infoln("Display initialized successfully");

    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 0);
    display.setTextSize(1);
    display.println("SENT");
#endif
}

static unsigned long last_updated = millis();

void display_progress_bar(int top, const unsigned short value, const unsigned short min = 0, unsigned short max = (ADC_MAX - 1))
{
#ifdef DISPLAY_ENABLED
    auto max_w = SCREEN_WIDTH - 1 - 2 * 2;
    auto actual_w = map(value, min, max, 0, max_w);

    //Log.warningln("disp: %d", actual_w);

    display.drawRect(2, top, max_w, 10, WHITE);
    display.fillRect(2, top, actual_w, 10, WHITE);
#endif
}

void display_update()
{
#ifdef DISPLAY_ENABLED
    auto now = millis();
    if ((now - last_updated) < 150) {   
        return;
    }
    last_updated = now;

    //display.clearDisplay();
    //display.drawRect(0, 0, 10, 10, WHITE);
    // display.setTextSize(1);
    // display.setTextColor(WHITE);
    // display.setCursor(0,0);

    display_progress_bar(2 + 30, display_rows[0]);

    display_progress_bar(14 + 30, display_rows[1]);

    display_progress_bar(26 + 30, display_rows[2]);

    display_progress_bar(38 + 30, display_rows[3]);

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