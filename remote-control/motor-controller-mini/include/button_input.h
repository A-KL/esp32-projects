#pragma once

#include <Arduino.h>

struct button_input_state_t {
    unsigned long last_press_ticks = 0;
};

struct button_counter_input_t {
    bool count_back = false;
    short d = 1;
};

struct button_input_t {
    const short gpio;
    const short active_state;
    const short long_press_delay_ms;
    const short start;
    const short end;
    short value;
    void (*value_changed_callback)(short);
    button_input_state_t state;
    button_counter_input_t counter;
};

inline bool is_active(const button_input_t& input, unsigned long ticks) {
    return ((ticks - input.state.last_press_ticks) >= input.long_press_delay_ms);
}

inline void button_input_init(const button_input_t& input) {
    pinMode(input.gpio, INPUT);
}

void button_input_update(button_input_t& input) 
{
    auto state = digitalRead(input.gpio);

    if (state) {
        delay(50);
        state = digitalRead(input.gpio);
    }

    auto ticks = millis();
    
    if (state == input.active_state) {
        if (input.state.last_press_ticks == 0) {
            input.state.last_press_ticks = ticks;
        } 
        else if (is_active(input, ticks)) {

            input.state.last_press_ticks = ticks;     
            input.value += input.counter.d;

            if (input.value == input.end) 
            {
                if (input.counter.count_back) 
                {
                    input.counter.d = -1;
                } 
                else 
                {
                    input.value = input.start;
                }
            } 
            else if (input.value == input.start) 
            {
                input.counter.d = 1;
            }

            input.value_changed_callback(input.value);
        }
    } else {
        input.state.last_press_ticks = 0;
    }
}