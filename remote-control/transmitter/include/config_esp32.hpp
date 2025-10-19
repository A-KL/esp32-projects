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

const int adc_input_pins[] = { 
    36, 39, // Left, Right Knobs
    34, 35, // Right Joystick - Y, X
    32, 33  // Left Joystick - Y, X  
};

const int sw_three_input_pins[] = { 
    2, 0, // Left Switch
    25, 26  // Right Switch
};

const int sw_two_input_pins[] = { 
    15, // Left Joystick - Button
    13, // Right Joystick - Button
};

constexpr int adc_input_pins_size = sizeof(adc_input_pins) / sizeof(int);
constexpr int sw_three_input_pins_size = sizeof(sw_three_input_pins) / sizeof(int);
constexpr int sw_two_input_pins_size = sizeof(sw_two_input_pins) / sizeof(int);