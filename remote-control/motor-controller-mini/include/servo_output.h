#pragma once

#include <Arduino.h>
#include <esp32-hal-log.h>
#include <servo.h>

static Servo servos[servos_count];

inline void servos_init() {
    for (auto i = 0; i < servos_count; i++) {
        servos[i].init(servos_pins[i], i, servos_inverted[i]);
    }
}

inline void servos_attach(bool state, uint8_t count = servos_count) {
    for (auto i = 0; i < count; i++) {
        servos[i].attach(state);
    }
}

template<int16_t TMin, int16_t TMax>
inline void servos_write(const int16_t* values, const uint8_t channels) 
{
#ifdef OUTPUT_SERVOS_DEBUG 
    log_d_values("[SERVO] ", values, min(channels, servos_count));
#endif
    for (auto i = 0; i < min(channels, servos_count); i++) {
         servos[i].write<TMin, TMax>(values[i]);
    }
}