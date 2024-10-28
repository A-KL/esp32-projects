
#pragma once

#include <vector>
#include <Arduino.h>

//https://github.com/contrem/arduino-timer
//#include <arduino-timer.h>
//Timer<10> timer; // 10 concurrent tasks, using millis as resolution
//timer.tick();

struct callback_info_t {
    unsigned long last_trigger;
    unsigned long period;
    void (*action_callback)(void);
};

static std::vector<callback_info_t> _scheduled;

void scheduler_add(unsigned long period_ms, void (*callback)(void)) {
    callback_info_t info { millis(), period_ms,  callback };
    _scheduled.push_back(info);
}

void scheduler_loop() {

    auto current = millis();

    for (callback_info_t& info : _scheduled) {
        if (info.last_trigger + info.period < current){
            info.last_trigger = current;
            info.action_callback();
        }
    }
}