#pragma once

class diagnostic_scope_t
{
    public:
        void begin() {
            _start_time = millis();
        }

        void finish(const char* name) {
            auto stop_time = millis();
            log_d("[%s] Elapsed: %d msec", name, stop_time - _start_time);
        }
        
    private:
        unsigned long _start_time = 0l;
};