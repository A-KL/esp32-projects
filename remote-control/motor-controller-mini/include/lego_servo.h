#pragma once

#include <math.h>
#include <Arduino.h>

#include <types.h>
#include <driver_limits.h>
#include <driver_config.h>

template<uint8_t PWM_CHANNELS_COUNT>
class EspArduinoHalDriver
{
protected:
    EspArduinoHalDriver()
    {

    }

    inline void init_pin(uint8_t pin, uint8_t direction) {
        pinMode(pin, direction);
    }

    inline void init_pwm_pin(uint8_t pin, uint8_t& channel = -1) {
        if (channel == -1)
        {
            channel = get_or_assign_channel(pin);
        }
        ledcAttachPin(pin, channel);
    }

    inline void write(uint8_t pin, int value, uint8_t& channel = -1) {
        if (channel == -1)
        {
            channel = get_or_assign_channel(pin);
        }
        ledcWrite(channel, value);
    }

    inline void write(uint8_t pin, bool value) {
        digitalWrite(pin, value);
    }



private:
    static uint8_t pwm_channels[PWM_CHANNELS_COUNT];

    int find_channel(uint8_t pin) {
        for (auto i=0; i<PWM_CHANNELS_COUNT; ++i) {
            if (pwm_channels[i] == pin) {
                return i;
            }
        }
    }

    int get_or_assign_channel(uint8_t pin) {
        for (auto i=0; i<PWM_CHANNELS_COUNT; ++i) {
            if (pwm_channels[i] == pin) {
                return i;
            }
        }
    }
};

class LegoServo
{
public:
    LegoServo(uint8_t pin_a, uint8_t pin_b, uint8_t pwm_channel) 
    : _pin_a(pin_a), _pin_b(_pin_b), _pwm_channel(pwm_channel)
    { }

    void init() {
        pinMode(_pin_a, OUTPUT);
        pinMode(_pin_b, OUTPUT);  
    }

    template<int16_t TMin, int16_t TMax>
    inline void write(int value)
    {
        //auto value = map();
        write(value);
    }

    void write(int value)
    {
        if (value == 0) 
        {
            if (_direction == backwards) {
                ledcDetachPin(_pin_a);
            } 
            else if(_direction = forward) {
                ledcDetachPin(_pin_b);
            }
            _direction = stop;
            digitalWrite(_pin_a, false);
            digitalWrite(_pin_b, false);
        } 
        else
        {
            if (_direction != forward && value < 0) {
                ledcDetachPin(_pin_a);
                ledcAttachPin(_pin_b, _pwm_channel);
                digitalWrite(_pin_a, false);
                _direction = forward;
            }
            else if (_direction != backwards && value > 0) {
                ledcDetachPin(_pin_b);
                ledcAttachPin(_pin_a, _pwm_channel);
                digitalWrite(_pin_b, false);
                _direction = backwards; 
            }

            ledcWrite(_pwm_channel, abs(value));                 
        }
    }

private:
    uint8_t _pin_a, _pin_b;
    uint8_t _pwm_channel;
    lego_servo_dir_t _direction = stop;
};

inline void lego_servo_init(const lego_servo_t& servo)
{
    pinMode(servo.pin_a, OUTPUT);
    pinMode(servo.pin_b, OUTPUT);
}

inline void lego_servos_init()
{
    for (auto& servo : lego_servos) {
        lego_servo_init(servo);
    }
}

void lego_servo_write(lego_servo_t& servo, int value)
{
    if (value == 0) 
    {
        if (servo.direction == forward) 
        {
            ledcDetachPin(servo.pin_a);
        } 
        else if(servo.direction = forward) 
        {
            ledcDetachPin(servo.pin_b);
        }

        servo.direction = stop;
        digitalWrite(servo.pin_a, false);
        digitalWrite(servo.pin_b, false);
    } 
    else
    {
        if (servo.direction != forward && value < 0) {
            ledcDetachPin(servo.pin_a);
            ledcAttachPin(servo.pin_b, servo.channel);
            digitalWrite(servo.pin_a, false);
            servo.direction = forward;
        }
        else if (servo.direction != backwards && value > 0) {
            ledcDetachPin(servo.pin_b);
            ledcAttachPin(servo.pin_a, servo.channel);
            digitalWrite(servo.pin_b, false);
            servo.direction = backwards; 
        }

        ledcWrite(servo.channel, abs(value));                 
    }
}

template<short TMin, short TMax>
inline void lego_servo_write(lego_servo_t& servo, int value)
{
    lego_servo_write(servo, map(value, TMin, TMax, LEGO_SERVO_LOW, LEGO_SERVO_HIGH));
}