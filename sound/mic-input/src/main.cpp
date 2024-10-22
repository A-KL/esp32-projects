#include <math.h>

#include "gui.h"
#include "audio.h"

enum audio_input_t {
    ADC = 0,
    I2S = 1, 
    Optical = 3
};

//#define DEBUG_OUTPUT

audio_input_t audio_input = I2S;

void IRAM_ATTR OnLeftButtonPressed()
{
    log_w("Left button Pressed!");
  //digitalWrite(LED_pin, !digitalRead(LED_pin));
}

void IRAM_ATTR OnRightButtonPressed()
{
    log_w("Right button Pressed!");
  //digitalWrite(LED_pin, !digitalRead(LED_pin));
}

void buttons_init(){
    pinMode(0, INPUT_PULLUP);
    pinMode(35, INPUT_PULLUP);
    attachInterrupt(0, OnLeftButtonPressed, RISING);
    attachInterrupt(35, OnRightButtonPressed, RISING);
}

void setup() 
{
    Serial.begin(115200);

    delay(1000);

    tft.init();
    tft.setRotation(TFT_ROTATE);
    tft.setSwapBytes(true);

    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.loadFont(NotoSansBold15);

    tft.fillScreen(TFT_BLACK);

    gui_init();
    gui_set_input((int)audio_input);

    envelope_init(right_envelope_context, I2S_SAMPLE_RATE);
    envelope_init(left_envelope_context, I2S_SAMPLE_RATE);

    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);

    buttons_init();

    xTaskCreate(gui_update_task, "gui_task", 2048, NULL, 1, NULL);
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