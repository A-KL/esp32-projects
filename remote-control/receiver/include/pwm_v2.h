#ifndef rc_pwm_v2_h
#define rc_pwm_v2_h

#include <Arduino.h>
#include <config_esp32_c3.h>

 #define COUNT_LOW 500
 #define COUNT_HIGH 2000

const int freq = 50;
const int timer_res = 14;

void pwm_init()
{
    for (int i = 0; i < pwm_channels_count; i++)
    {
        ledcSetup(i, freq, timer_res);
        ledcAttachPin(pwm_pins[i], i);
    }
}

void pwm_start() {
}

void pwm_stop() {
    for (int i = 0; i < pwm_channels_count; i++)
    {
        ledcDetachPin(pwm_pins[i]);
    }
}

void pwm_write(const int values[], int size) {
    for (int i = 0; i < size; i++) {
         ledcWrite(i, 0);
    }
}

void pwm_write(const int ch, const int value) {
    ledcWrite(ch, map(value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, COUNT_LOW, COUNT_HIGH));
}

#endif