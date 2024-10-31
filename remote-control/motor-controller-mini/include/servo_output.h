#pragma once

#include <Arduino.h>
#include <esp32-hal-log.h>

#define SERVO_FREQ        50
#define SERVO_RES         14
#define SERVO_DUTY_CYCLE  ((long)((1<<SERVO_RES)-1))
#define SERVO_LOW         ((long)(SERVO_DUTY_CYCLE * 0.025))
#define SERVO_HIGH        ((long)(SERVO_DUTY_CYCLE * 0.125))

class Servo
{
public:

    inline void init(uint8_t pin, uint8_t channel)
    {
        _channel = channel;
        _pin = pin;

        assert(ledcSetup(_channel, SERVO_FREQ, SERVO_RES));
        _init = true;
    }

    void attach(bool attach = true)
    {
        if (attach && !_attached) {
            ledcAttachPin(_pin, _channel);
        }
        if (!attach && _attached) {
            ledcDetachPin(_pin);
        }
        _attached = attach;
    }

    // 0 - 180 - 270
    inline void write_angle(uint8_t angle) const
    {
        write<0,180>(angle);
    }

    template<uint8_t TMin, uint8_t TMax>
    inline void write(uint8_t angle) const
    {
        if (!_attached) {
            return;
        }
        angle = constrain(angle, TMin, TMax);
        ledcWrite(_channel, map(angle, TMin, TMax, SERVO_LOW, SERVO_HIGH));
    }

private:
    uint8_t _pin = 0;
    int8_t _channel = 0;
    bool _init = false;
    bool _attached = false;
};

// Servos

static Servo servos[servos_count] = {};

inline void servos_init() {
    for (auto i = 0; i < servos_count; i++) {
        servos[i].init(servos_pins[i], i);
    }
}

inline void servos_attach(bool state) {
    for (auto i = 0; i < servos_count; i++) {
        servos[i].attach(state);
    }
}

template<int16_t TMin, int16_t TMax>
inline void servos_write(const int16_t* values, const uint8_t channels) {
    for (auto i = 0; i < min(channels, servos_count); i++) {
         servos[i].write<TMin, TMax>(values[i]);
    }
}