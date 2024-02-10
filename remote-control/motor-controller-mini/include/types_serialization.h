#pragma once
	
// #include <map>
// #include <vector>
// #include <Arduino.h>

// concept
// namespace outputs {
//   enum output_type_t : uint8_t {
//     pwm   = 0,
//     motor = 1,
//     sbus  = 2
//   };
// }

// const std::map<String, outputs::output_type_t> drive_output_map = 
// { 
//   { "pwm", outputs::pwm },
//   { "motors", outputs::motor }, 
//   { "sbus", outputs::sbus } 
// };

// typedef struct {
//   uint8_t type;
//   uint8_t size;
// } node_config_t;

// const node_config_t input_configs[] = {
//   { inputs::pwm, 2 },
//   { inputs::adc, 2 },
//   { inputs::sbus, 16 },
//   { inputs::now, 16 }
// };

// const node_config_t outputs_configs[] = {
//   { outputs::pwm, 2 },
//   { outputs::motor, 2 },
//   { outputs::sbus, 16 }
// };

// typedef struct {
//     const node_config_t input;
//     const node_config_t output;
// } mapping_config_t;

// std::vector<mapping_config_t> mappings = {
//   { {inputs::sbus, 0}, {outputs::motor, 0} },
//   { {inputs::sbus, 1}, {outputs::motor, 1} },

//   { {inputs::now, 0}, {outputs::motor, 0} },
//   { {inputs::now, 1}, {outputs::motor, 1} },
// };