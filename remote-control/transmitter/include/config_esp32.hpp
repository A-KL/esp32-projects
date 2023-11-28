#pragma once

#define LED_BUILTIN 5

#define RF_CE_PIN 16
#define RF_CSN_PIN 17

#define ADC_MIN 0
#define ADC_MAX 4095

#define INPUT_SBUS_MIN  200
#define INPUT_SBUS_MAX  1800

#define INPUT_PWM_MAX   2000
#define INPUT_PWM_MIN   1024

#define ADC_CHANNELS_COUNT 6
#define SW_TWO_CHANNELS_COUNT 2
#define SW_THREE_CHANNELS_COUNT 2

int adc_input_pins[] = { 
    36, 39, // Left, Right Knobs
    34, 35, // Right Joystick - Y, X
    32, 33  // Left Joystick - Y, X  
};

int sw_three_input_pins[SW_THREE_CHANNELS_COUNT * 2] = { 
    2, 0, // Left Switch
    25, 26  // Right Switch
};

int sw_two_input_pins[SW_TWO_CHANNELS_COUNT] = { 
    15, // Left Joystick - Button
    13, // Right Joystick - Button
};