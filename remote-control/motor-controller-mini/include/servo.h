#pragma once

#include <Arduino.h>

#define OUTPUT_SERVO_FREQ     50
#define OUTPUT_SERVO_RES      10

class Servo
{
public:
    inline bool init(uint8_t pin, uint8_t channel, bool reverse = false)
    {
        _pin = pin;
        _channel = channel;
        _reverse = reverse;
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
    void write(int16_t value) const
    {
        write(TMin, TMax, value);
    }

    void write(int16_t TMin, int16_t TMax, int16_t value) const
    {
        if (!_attached || !_init) {
            return;
        }

        value = constrain(value, TMin, TMax);

        auto TMid = (TMin + TMax) / 2;
        auto value_mapped = servo_mid;

        // if (value < TMid) 
        // {
        //     value_mapped = map(value,
        //         _reverse ? TMid : TMin,
        //         _reverse ? TMin : TMid, 
        //         servo_low,   
        //         servo_mid);
        // } 
        // else if (value > TMid)
        // {
        //     value_mapped = map(value,
        //         _reverse ? TMax : TMid,
        //         _reverse ? TMid : TMax,
        //         servo_mid,
        //         servo_high);
        //    // log_d("value_mapped %u", value_mapped);
        // }

         if (value != TMid)  {
            value_mapped = map(value, 
                _reverse ? TMax : TMin,
                _reverse ? TMin : TMax,
                servo_low,
                servo_high);
           // log_d("value_mapped %u", value_mapped);
        }

        ledcWrite(_channel, value_mapped);
    }

private:
    int8_t _pin = -1;
    int8_t _channel = -1;

    bool _init = false;
    bool _attached = false;
    bool _reverse = false;

    const uint8_t servo_frequency = OUTPUT_SERVO_FREQ;
    const uint8_t servo_resolution = OUTPUT_SERVO_RES;
    const uint32_t servo_duty_cycle = ((1<<servo_resolution) - 1);

    // 0.5ms pulse width (360 degree) 0.5/20 = 0.025
    // 1.0ms pulse width (180 degree) 1.0/20 = 0.05
    const uint32_t servo_low = (servo_duty_cycle * 0.050); //49 // left

    // 
    const uint32_t servo_mid = (servo_duty_cycle * 0.076); //   mid

    // 2.5ms pulse width (360 degree) 2.5/20 = 0.125
    // 2.0ms pulse width (180 degree) 2.0/20 = 0.1
    const uint32_t servo_high = (servo_duty_cycle * 0.1); // 105 //right
};
