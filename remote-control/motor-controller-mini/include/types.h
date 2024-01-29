#pragma once
	
#include <map>

enum motor_drive_mode_t {
  a_b_en  = 0,
  a_b     = 1,
  dir_en  = 2
};

const std::map<String, motor_drive_mode_t> drive_modes_map = 
{ 
  {"a_b_en", a_b_en}, 
  {"a_b", a_b}, 
  {"dir_en", dir_en} 
};

namespace inputs {
  enum input_type_t : uint8_t {
    pwm  = 0,
    adc  = 1,
    sbus = 2,
    now = 3
  };
}

const std::map<String, inputs::input_type_t> drive_input_map = 
{ 
  { "pwm", inputs::input_type_t::pwm }, 
  { "adc", inputs::input_type_t::adc }, 
  { "sbus", inputs::input_type_t::sbus },
  { "now", inputs::input_type_t::now }
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
    motor_drive_mode_t mode;
    bool inverted;
    inputs::input_type_t input_type;
    int input_channel;
} motor_config_t;

// concept
namespace outputs {
  enum output_type_t : uint8_t {
    pwm   = 0,
    motor = 1,
    sbus  = 2
  };
}

const std::map<String, outputs::output_type_t> drive_output_map = 
{ 
  { "pwm", outputs::output_type_t::pwm },
  { "motors", outputs::output_type_t::motor }, 
  { "sbus", outputs::output_type_t::sbus } 
};

typedef struct {
  uint8_t type;
  uint8_t size;
} node_config_t;

const node_config_t input_configs[] = {
  { inputs::input_type_t::pwm, 2 },
  { inputs::input_type_t::adc, 2 },
  { inputs::input_type_t::sbus, 16 },
  { inputs::input_type_t::now, 16 }
};

const node_config_t outputs_configs[] = {
  { outputs::output_type_t::pwm, 2 },
  { outputs::output_type_t::motor, 2 },
  { outputs::output_type_t::sbus, 16 }
};

typedef struct {
    const node_config_t& input;
    const node_config_t& output;
} mapping_config_t;