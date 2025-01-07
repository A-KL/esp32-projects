#pragma once

#include <Arduino.h>

template<typename T>
class driver_strategy_t
{
    public:
        driver_strategy_t(T min_speed, T max_speed, T start = 0) 
            : _min_speed(min_speed), _max_speed(max_speed), _actual_speed(start), _desired_speed(start)
        {}

        inline void write(T desired) 
        {
            if (_desired_speed == desired) 
            {
                return;
            }
            _desired_speed_updated = millis();
            _desired_speed = desired;
            _acc_step *= (_desired_speed - _actual_speed) / abs(_desired_speed - _actual_speed);
        }

        inline T read()
        {
            if (_actual_speed != _desired_speed) 
            {
                auto actual = millis();

                if (actual - _desired_speed_updated > _acc_ms) 
                {
                    _desired_speed_updated = actual;
                    _actual_speed = constrain(_actual_speed + _acc_step, _min_speed, _max_speed);
                }
            }
            return _actual_speed;
        }

    private:
        long _desired_speed_updated;

        uint16_t _acc_ms = 50;

        T _acc_step = 10;

        T _actual_speed;
        T _desired_speed;
        
        T _min_speed;
        T _max_speed;
};

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

