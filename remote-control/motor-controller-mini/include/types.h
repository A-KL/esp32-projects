#pragma once

#include <map>
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
  enum motor_drive_mode_t mode;
  bool inverted;
  enum input_type_t input_type;
  int input_channel;
};

const std::map<const String, const motor_drive_mode_t> drive_modes_map = 
{ 
  {"a_b_en", a_b_en}, 
  {"a_b", a_b}, 
  {"dir_en", dir_en} 
};

const std::map<const String, const input_type_t> drive_input_map = 
{ 
  { "pwm", pwm }, 
  { "adc", adc }, 
  { "sbus", sbus },
  { "now", now }
};

enum lego_servo_dir_t {
    left = -1,
    center = 0,
    right = 1
};

struct lego_servo_t {
    short pin_a;
    short pin_b;
    short channel;
    lego_servo_dir_t direction;
};

// v2

enum output_type_t {
  motor  = 0,
  servo  = 1,
  servo_lego = 2
};

const std::map<const String, const output_type_t> outputs_map = 
{ 
  { "dc motor", motor }, 
  { "servo", servo }, 
  { "lego servo", servo_lego }
};

struct output_config_t
{
  output_type_t type;
  int channel;
};

struct input_config_t
{
  input_type_t type;
  int channel;
};

typedef std::map<const output_config_t, const input_config_t[]> global_config_t;

// const String* output_to_string(const output_type_t type) 
// {
//     auto iter = outputs_map.find(type);

//         if (input_type_iter == drive_input_map.end())
//         {
//   return outputs_map
// }