#pragma once

#include <Arduino.h>

struct driver_config_t
{
    int16_t pin_speed;
    int16_t direction_pin;
    int16_t brake_pin = -1;
};

inline void driver_init(int16_t pin_direction, int16_t pin_speed, int16_t freq = 10000, int16_t pin_brake = -1) 
{
    pinMode(pin_direction, OUTPUT);
    pinMode(pin_speed, OUTPUT);

    if (pin_brake > -1) {
        pinMode(pin_brake, OUTPUT);
    }
}

