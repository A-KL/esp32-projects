void gui_progress_bars_update();

#ifdef ARDUINO
    #include <Arduino.h>
    #ifdef ESP_PLATFORM
        #include "hal_esp32.h"
    #endif
#else
    #include "hal_arduino.h"
#endif

//#include <LGFX_TFT_eSPI.hpp>
#include <LovyanGFX.h>
#include <LGFX_AUTODETECT.hpp>
using TFT_eSPI = LGFX;

#include <TFT_eSprite_GFX.h>
using TFT_eSprite = TFT_eSprite_GFX;

#include <TFT_eGUI.h>

#if defined ( SDL_h_ )
    static TFT_eSPI tft ( 320, 240, 2 );
#else
    static TFT_eSPI tft;
#endif

#include <math.h>
#include "gui.h"
#include "audio.h"
#include "network.h"

enum audio_input_t {
    ADC = 0,
    I2S = 1, 
    Optical = 3
};

audio_input_t audio_input = I2S;

void setup() 
{
    hal_init();

    gui_init();
    gui_set_input((int)audio_input);

    hal_audio_init();

    hal_buttons_init();

    hal_gui_start();
}

void loop() 
{
#ifdef DEBUG_OUTPUT
    auto rangelimit = 3000;
    Serial.print(rangelimit * -1);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");
#endif

    switch (audio_input)
    {
        case ADC:
            update_analog();
            break;
        
        case I2S:
            update_i2s();
        
        default:
            break;
    }

    // vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half of the cpu cycles
}