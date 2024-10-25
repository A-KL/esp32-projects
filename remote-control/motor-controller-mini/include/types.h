#pragma once

#include <map>
#include <vector>
#include <Arduino.h>

enum motor_mode_t {
  a_b_en  = 0,
  a_b     = 1,
  dir_en  = 2
};

const std::map<const String, const motor_mode_t> drive_modes_map = 
{ 
  {"a_b_en", a_b_en}, 
  {"a_b", a_b},
  {"dir_en", dir_en} 
};

struct motor_pins_t 
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
  motor_mode_t mode;
  motor_pins_t pins;
};

enum lego_servo_dir_t {
    forward = 0,
    stop = 1,
    backwards = 2
};

struct lego_servo_t {
    short pin_a;
    short pin_b;
    short channel;
    lego_servo_dir_t direction;
};

// Input config

enum output_type_t {
  motor  = 0,
  servo  = 1,
  servo_lego = 2
};

const std::map<const String, const output_type_t> outputs_map = 
{ 
  { "motor", motor }, 
  { "servo", servo }, 
  { "servo_lego", servo_lego }
};

// const String* output_to_string(const output_type_t type) 
// {
//     auto iter = outputs_map.find(type);

//         if (input_type_iter == drive_input_map.end())
//         {
//   return outputs_map
// }

inline const output_type_t string_to_output_type(const String value) 
{
    auto iter = outputs_map.find(value);

    return (iter == outputs_map.end() ? (output_type_t)-1 : iter->second);
}

struct input_config_t
{
  output_type_t out_type;
  int in_channel;
  int out_channel;
};

typedef std::map<const String, std::vector<input_config_t>> global_config_t;

// Vars
global_config_t global_config;

// v2

//global_config_t global_config;

// const std::map<const String, const input_type_t> drive_input_map = 
// { 
//   { "pwm", pwm }, 
//   { "adc", adc }, 
//   { "sbus", sbus },
//   { "now", now }
// };

// struct output_config_t
// {
//   output_type_t type;
//   int channel;
// };

// struct input_config_t
// {
//   input_type_t type;
//   int channel;
// };