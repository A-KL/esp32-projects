#pragma once

#include <map>
#include <vector>
#include <Arduino.h>

// Outputs

enum output_type_t {
  dc         = 0,
  servo      = 1,
  servo_lego = 2,
  sbus_out   = 3
};

enum dc_mode_t {
  a_b_en  = 0,
  a_b     = 1,
  dir_en  = 2
};

struct dc_pins_t 
{
  short a;
  short b;
  short en;

  short a_channel;
  short b_channel;
  short en_channel;
};

struct motor_config_t
{
  dc_mode_t mode;
  dc_pins_t pins;
};

enum lego_servo_position_t {
    forward = 0,
    middle = 1,
    backwards = 2
};

struct lego_servo_t {
    short pin_a;
    short pin_b;
    short channel;
    lego_servo_position_t direction; // = lego_servo_position_t::middle;
};

// Input config

enum input_type_t {
  none    = -1,
  pwm     = 0,
  adc     = 1,
  sbus    = 2,
  esp_now = 3,
  ps3     = 4,
  xbox    = 5
};

// Global

struct input_config_t
{
  output_type_t out_type;
  int in_channel;
  int out_channel;
};

typedef std::map<const input_type_t, std::vector<input_config_t>> global_config_t;