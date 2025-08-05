#pragma once

#include <math.h>
#include <Arduino.h>
#include <types.h>

#define LEGO_SERVO_FREQ     50
#define LEGO_SERVO_RES      10

#define LEGO_SERVO_MIN      0
#define LEGO_SERVO_MAX      (long)(pow(2, LEGO_SERVO_RES) - 1)

class lego_servo_driver_t
{
    public:
        lego_servo_driver_t(uint8_t pin_a, uint8_t pin_b, uint8_t pwm_channel) 
            : _pin_a(pin_a), _pin_b(_pin_b), _pwm_channel(pwm_channel)
        { }

        void init() {
            pinMode(_pin_a, OUTPUT);
            pinMode(_pin_b, OUTPUT);  
        }

        bool attach(bool attach = true)
        {
            if (_state == middle) {
                return true;
            }

            if (!attach && _state == forward) {
                ledcDetachPin(_pin_b);
            }

            if (!attach && _state == backwards) {
                ledcDetachPin(_pin_a);
            }

            return true;
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
                if (_state == backwards) {
                    ledcDetachPin(_pin_a);
                } 
                else if(_state = forward) {
                    ledcDetachPin(_pin_b);
                }
                _state = middle;
                digitalWrite(_pin_a, false);
                digitalWrite(_pin_b, false);
            } 
            else
            {
                if (_state != forward && value < 0) {
                    ledcDetachPin(_pin_a);
                    ledcAttachPin(_pin_b, _pwm_channel);
                    digitalWrite(_pin_a, false);
                    _state = forward;
                }
                else if (_state != backwards && value > 0) {
                    ledcDetachPin(_pin_b);
                    ledcAttachPin(_pin_a, _pwm_channel);
                    digitalWrite(_pin_b, false);
                    _state = backwards; 
                }

                ledcWrite(_pwm_channel, abs(value));                 
            }
        }

    private:
        uint8_t _pin_a, _pin_b;
        uint8_t _pwm_channel;
        lego_servo_position_t _state = middle;
};

void lego_servo_init(const lego_servo_t& servo)
{
    pinMode(servo.pin_a, OUTPUT);
    pinMode(servo.pin_b, OUTPUT);
    ledcSetup(servo.channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
}

void lego_servos_init()
{
    for (auto& servo : lego_servos) {
        lego_servo_init(servo);
    }
}

void lego_servo_write(lego_servo_t& servo, int16_t value)
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

        servo.direction = middle;
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

template<int TMin, int TMax>
inline void lego_servo_write(lego_servo_t& servo, int16_t value)
{
    lego_servo_write(servo, map(value, TMin, TMax, LEGO_SERVO_MIN, LEGO_SERVO_MAX));
}

inline void lego_servo_write(uint8_t index, int16_t output)
{
    lego_servo_write(lego_servos[index], output);
}

template<int TMin, int TMax>
inline void lego_servo_write(uint8_t index, int16_t output)
{
    lego_servo_write<TMin, TMax>(lego_servos[index], output);
}

void lego_servos_write(int16_t* outputs, uint8_t count)
{
  for (auto i = 0; i<count; ++i) {
    lego_servo_write(i, outputs[i]);
  }
}

template<int TMin, int TMax>
void lego_servos_write(int16_t* outputs, uint8_t count)
{
  for (auto i = 0; i<count; ++i) {
    lego_servo_write<TMin, TMax>(i, outputs[i]);
  }
}