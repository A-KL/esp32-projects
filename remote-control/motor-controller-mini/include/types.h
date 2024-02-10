#pragma once

#include <map>
#include <vector>
#include <Arduino.h>

enum motor_drive_mode_t {
  a_b_en  = 0,
  a_b     = 1,
  dir_en  = 2
};

enum input_type_t {
  pwm  = 0,
  adc  = 1,
  sbus = 2,
  now = 3
};

typedef struct {
  short a;
  short b;
  short en;

  short a_channel;
  short b_channel;
  short en_channel;
} motor_pins_t;

typedef struct {
  enum motor_drive_mode_t mode;
  bool inverted;
  enum input_type_t input_type;
  int input_channel;
} motor_config_t;

const std::map<String, motor_drive_mode_t> drive_modes_map = 
{ 
  {"a_b_en", a_b_en}, 
  {"a_b", a_b}, 
  {"dir_en", dir_en} 
};

const std::map<String, input_type_t> drive_input_map = 
{ 
  { "pwm", pwm }, 
  { "adc", adc }, 
  { "sbus", sbus },
  { "now", now }
};