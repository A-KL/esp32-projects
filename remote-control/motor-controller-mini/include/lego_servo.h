#pragma once

#include <math.h>
#include <Arduino.h>

#include <types.h>
#include <config_esp32.h>

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
        auto value = map();
        write(value);
    }

    void write(int value)
    {
        if (value == 0) 
        {
            if (_direction == left) {
                ledcDetachPin(_pin_a);
            } 
            else if(_direction = right) {
                ledcDetachPin(_pin_b);
            }
            _direction = center;
            digitalWrite(_pin_a, false);
            digitalWrite(_pin_b, false);
        } 
        else
        {
            if (_direction != right && value < 0) {
                ledcDetachPin(_pin_a);
                ledcAttachPin(_pin_b, _pwm_channel);
                digitalWrite(_pin_a, false);
                _direction = right;
            }
            else if (_direction != left && value > 0) {
                ledcDetachPin(_pin_b);
                ledcAttachPin(_pin_a, _pwm_channel);
                digitalWrite(_pin_b, false);
                _direction = left; 
            }

            ledcWrite(_pwm_channel, abs(value));                 
        }
    }

private:
    uint8_t _pin_a, _pin_b;
    uint8_t _pwm_channel;
    lego_servo_dir_t _direction = center;
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
        if (servo.direction == left) {
            ledcDetachPin(servo.pin_a);
        } 
        else if(servo.direction = right) {
            ledcDetachPin(servo.pin_b);
        }
        servo.direction = center;
        digitalWrite(servo.pin_a, false);
        digitalWrite(servo.pin_b, false);
    } 
    else
    {
        if (servo.direction != right && value < 0) {
            ledcDetachPin(servo.pin_a);
            ledcAttachPin(servo.pin_b, servo.channel);
            digitalWrite(servo.pin_a, false);
            servo.direction = right;
        }
        else if (servo.direction != left && value > 0) {
            ledcDetachPin(servo.pin_b);
            ledcAttachPin(servo.pin_a, servo.channel);
            digitalWrite(servo.pin_b, false);
            servo.direction = left; 
        }

        ledcWrite(servo.channel, abs(value));                 
    }
}

template<short TMin, short TMax>
inline void lego_servo_write(lego_servo_t& servo, int value)
{
    lego_servo_write(servo, map(value, TMin, TMax, LEGO_SERVO_LOW, LEGO_SERVO_HIGH));
}