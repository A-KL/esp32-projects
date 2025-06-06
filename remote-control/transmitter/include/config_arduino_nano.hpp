#pragma once

#include <Arduino.h>

// #define RF_CE_PIN 7
// #define RF_CSN_PIN 8

#define RF_CE_PIN 10
#define RF_CSN_PIN 9

#define ADC_MIN 0
#define ADC_MAX 1024

const int adc_input_pins[] = { 
    A0, A1, A2
};

const int sw_three_input_pins[] = { 
};

const int sw_two_input_pins[] = { 
};

constexpr int adc_input_pins_size = sizeof(adc_input_pins) / sizeof(int);
constexpr int sw_three_input_pins_size = sizeof(sw_three_input_pins) / sizeof(int);
constexpr int sw_two_input_pins_size = sizeof(sw_two_input_pins) / sizeof(int);