#pragma once

#include <math.h>
#include <Arduino.h>

#include <types.h>
#include <config_esp32.h>

void lego_servo_init(const lego_servo_t& servo)
{
    pinMode(servo.pin_a, OUTPUT);
    pinMode(servo.pin_b, OUTPUT);
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
void lego_servo_write(lego_servo_t& servo, int value)
{
    lego_servo_write(servo, map(value, TMin, TMax, LEGO_SERVO_LOW, LEGO_SERVO_HIGH));
}