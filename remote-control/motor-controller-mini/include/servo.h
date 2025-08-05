#pragma once

#include <Arduino.h>

#define SERVO_FREQ     50
#define SERVO_RES      14

class Servo
{
public:
    inline bool init(uint8_t pin, uint8_t channel)
    {
        _channel = channel;
        _pin = pin;
        _init = ledcSetup(_channel, servo_frequency, servo_resolution) != 0;
        return _init;
    }

    bool attach(bool attach = true)
    {
        if (!_init) {
            return false;
        }

        if (attach && !_attached) {
            ledcAttachPin(_pin, _channel);
        }
        if (!attach && _attached) {
            ledcDetachPin(_pin);
        }

        _attached = attach;
        return true;
    }

    // 0 - 180 - 270
    inline void write_angle(uint8_t angle) const
    {
        write<0,180>(angle);
    }

    template<int16_t TMin, int16_t TMax>
    inline void write(int16_t value) const
    {
        if (!_attached || !_init) {
            return;
        }
        ledcWrite(_channel, map(constrain(value, TMin, TMax), TMin, TMax, servo_low, servo_high));
    }

private:
    int8_t _pin = -1;
    int8_t _channel = -1;

    bool _init = false;
    bool _attached = false;

    const uint8_t servo_frequency = SERVO_FREQ;
    const uint8_t servo_resolution = SERVO_RES;
    const uint32_t servo_duty_cycle = ((1<<servo_resolution) - 1);
    const uint32_t servo_low = (servo_duty_cycle * 0.025);
    const uint32_t servo_high = (servo_duty_cycle * 0.125);
};
