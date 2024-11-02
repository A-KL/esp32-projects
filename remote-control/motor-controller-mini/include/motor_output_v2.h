#pragma once

#include <driver_config.h>
#include <motor.h>

void trace_values(const char* label, const uint16_t* outputs, const uint8_t count)
{
  char trace[100];
  for (auto i = 0; i<count; ++i) {
    sprintf(trace, "%s\t%d", trace, outputs[i]);
  }
  log_d("%s (%s)", label, trace);
}


// #ifdef OUTPUT_MOTOR_DEBUG
//    log_d("[MOTOR] (%d) Speed: %d", config.mode, speed);
// #endif

// static Motor motors[motors_count] = {};

// void motors_init() 
// {
//   for (auto i=0; i<motors_count; ++i) {
//       motors->pins(motors[i]);
//       motors.write(motors[i], 0);
//   }
// }