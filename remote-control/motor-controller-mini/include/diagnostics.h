// #pragma once

// #include <Timer.h>

// class diagnostic_scope_t
// {
//     public:
//         void begin() {
//             _timer.start();
//         }

//         void finish(const char* name) {
//             _timer.stop();
//             log_d("[%s] Elapsed: %d msec", name, _timer.read());
//         }
        
//     private:
//         Timer _timer;
// };