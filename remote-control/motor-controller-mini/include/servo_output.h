#pragma once

#include <Arduino.h>
#include <esp32-hal-log.h>
#include <ServoSmooth.h>

// #include "soc/mcpwm_reg.h"
// #include "soc/mcpwm_struct.h"
// #include "driver/mcpwm.h"

class MyServo
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
    inline void write_angle(uint8_t angle)
    {
        write<0,180>(angle);
    }

    template<int16_t TMin, int16_t TMax>
    inline void write(int16_t value)
    {
        if (!_init) {
            return;
        }
        if (!_attached) {
            attach();
        }
        ledcWrite(_channel, map(constrain(value, TMin, TMax), TMin, TMax, servo_low, servo_high));
    }

private:
    int8_t _pin = -1;
    int8_t _channel = -1;

    bool _init = false;
    bool _attached = false;

    const uint8_t servo_frequency = 50;
    const uint8_t servo_resolution = 14;
    const uint32_t servo_duty_cycle = ((1<<servo_resolution) - 1);
    const uint32_t servo_low = 500;// (servo_duty_cycle * 0.025);
    const uint32_t servo_high = 2500;// (servo_duty_cycle * 0.125);
};

// Servos

const uint8_t servo_frequency = 50;
const uint8_t servo_resolution = 14;
const uint32_t servo_duty_cycle = ((1<<servo_resolution) - 1);
const uint32_t servo_low = (servo_duty_cycle * 0.025);
const uint32_t servo_high = (servo_duty_cycle * 0.125);

static ServoSmooth servos[servos_count];

inline void servos_init() {
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 0);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, 1); 

    for (auto i = 0; i < servos_count; i++) {
       // servos[i].init(servos_pins[i], i);
       //servos[i].begin();
    }
}

inline void servos_attach(bool state, uint8_t count = servos_count) {
    // for (auto i = 0; i < count; i++) {
    //     servos[i].attach(state);
    // }
    for (auto i = 0; i < count; i++) 
    {
        if (state)
        {
            servos[i].attach(servos_pins[i], 1000, 2000); // Attach the servo after it has been detatched
            servos[i].setSpeed(130); // ограничить скорость
            servos[i].setAccel(0.3);   // установить ускорение (разгон и торможение)
            servos[i].smoothStart();
        }
        else
        {
            servos[i].detach();
        }
    }
}

template<int16_t TMin, int16_t TMax>
inline void servos_write(const int16_t* values, const uint8_t channels) 
{
#ifdef OUTPUT_SERVOS_DEBUG 
    trace_values("[SERVOS] ", values, min(channels, servos_count));
#endif
    // for (auto i = 0; i < min(channels, servos_count); i++) {
    //      servos[i].write<TMin, TMax>(values[i]);
    // }
    for (auto i = 0; i < min(channels, servos_count); i++) {
         servos[i].writeMicroseconds(map(values[i], TMin, TMax,  MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
    }
}

inline void servos_loop()
{
    for (auto i = 0; i < servos_count; i++) {
      servos[i].tickManual();
    }
}