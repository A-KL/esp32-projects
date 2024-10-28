#pragma once

#include <Arduino.h>
#include <esp32-hal-log.h>

#define SERVO_FREQ        50
#define SERVO_RES         14
#define SERVO_DUTY_CYCLE  ((long)((1<<SERVO_RES)-1))
#define SERVO_LOW         ((long)(SERVO_DUTY_CYCLE * 0.025))
#define SERVO_HIGH        ((long)(SERVO_DUTY_CYCLE * 0.125))

// PWM

inline void pwm_init(const uint8_t channel, const int16_t freq, const int16_t res) {
    assert(ledcSetup(channel, freq, res));
}

inline void pwm_start(const uint8_t channel, const uint8_t pin) {
    ledcAttachPin(pin, channel);
}

inline void pwm_stop(const uint8_t pin) {
    ledcDetachPin(pin);
}

inline void pwm_write(const uint8_t channel, const int16_t value) {
    ledcWrite(channel, value);
}

template<int16_t TMin, int16_t TMax>
inline void pwm_write(const uint8_t channel, const int16_t value) {
    ledcWrite(channel, map(value, TMin, TMax, SERVO_LOW, SERVO_HIGH));
}

inline void pwm_write(const int16_t* values, const uint8_t channels) {
    for (auto i = 0; i < min(channels, servos_count); i++) {
         pwm_write(i, values[i]);
    }
}

template<int16_t TMin, int16_t TMax>
inline void pwm_write(const int16_t* values, const uint8_t channels) {
    for (auto i = 0; i < min(channels, servos_count); i++) {
         pwm_write<TMin, TMax>(i, values[i]);
    }
}

// Servos

inline void servos_init() {
    for (int i = 0; i < servos_count; i++) {
        pwm_init(i, SERVO_FREQ, SERVO_RES); // first is channel
    }
}

inline void servos_start() {
    for (auto i = 0; i < servos_count; i++) {
        pwm_start(i, servos_pins[i]);
    }
}

inline void servos_stop() {
    for (auto i = 0; i < servos_count; i++) {
        pwm_stop(servos_pins[i]);
    }
}