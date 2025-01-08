#pragma once

#include <Arduino.h>

template<typename T>
class driver_strategy_t
{
    public:
        driver_strategy_t(T min_speed, T max_speed, T start = 0) 
            : _min_speed(min_speed), _max_speed(max_speed), _actual_speed(start), _desired_speed(start), _actual_updated(millis())
        {}

        inline void write(T desired) 
        {
            if (desired == _desired_speed) {
                return;
            }
            _desired_speed = desired;
        }

        inline T read()
        {
            if (_actual_speed != _desired_speed) 
            {
                auto actual_updated = millis();

                if (actual_updated - _actual_updated > _acc_ms) 
                {
                    _actual_updated = actual_updated;

                    auto actual_step = _acc_step;

                    if (_desired_speed < _actual_speed)
                    {
                        actual_step *= -1;
                    }
                    _actual_speed = constrain(_actual_speed + actual_step, _min_speed, _max_speed);
                }
            }
            return _actual_speed;
        }

    private:
        long _actual_updated;

        const uint16_t _acc_ms = 50;

        const T _acc_step = 5;

        T _actual_speed;
        volatile T _desired_speed;
        
        const T _min_speed;
        const T _max_speed;
};
