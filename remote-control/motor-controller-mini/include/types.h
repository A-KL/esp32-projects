#ifndef types_h
#define types_h
	
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
  const std::map<String, input_type_t> drive_input_map = 
  { 
    { "pwm", pwm }, 
    { "adc", adc }, 
    { "sbus", sbus },
    { "now", now }
  };
}

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
  const std::map<String, output_type_t> drive_input_map = 
  { 
    {"pwm", pwm},
    {"motors", motor}, 
    {"sbus", sbus} 
  };
}

typedef struct {
    uint8_t type;
    uint8_t size;
} node_config_t;

const node_config_t input_configs[] = {
  { inputs::pwm, 2 },
  { inputs::adc, 2 },
  { inputs::sbus, 16 },
  { inputs::now, 16 }
};

const node_config_t outputs_configs[] = {
  { outputs::pwm, 2 },
  { outputs::motor, 2 },
  { outputs::sbus, 16 }
};

typedef struct {
    const node_config_t& input;
    const node_config_t& output;
} mapping_config_t;

#endif