#ifndef rc_pwm_h
#define rc_pwm_h

#include <ESP32Servo.h>
#include <config_esp32_c3.h>

Servo servos[pwm_channels_count];

void pwm_init()
{
    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    for (int i = 0; i < pwm_channels_count; i++)
    {
        if (!ESP32PWM::hasPwm(pwm_pins[i])){
            log_w("Pin %d doesn't support PWM",i);
        }
        else
        {
            servos[i].setPeriodHertz(50);
        }
    }
}

void pwm_start() {
    for (int i = 0; i < pwm_channels_count; i++) {
        if (!servos[i].attached()) {
            servos[i].attach(pwm_pins[i], INPUT_PWM_MIN, INPUT_PWM_MAX);
        }
    }
}

void pwm_stop() {
    for (int i = 0; i < sizeof(servos); i++) {
        if (servos[i].attached()) {
            servos[i].detach();
        }
    }
}

void pwm_write(const int values[], int size) {
    for (int i = 0; i < size; i++) {
        if (servos[i].attached()) {
            servos[i].attach(pwm_pins[i], INPUT_PWM_MIN, INPUT_PWM_MAX);
        }
        servos[i].write(values[i]);
    }
}

#endif