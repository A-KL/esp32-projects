#ifndef types_h
#define types_h

enum motor_drive_mode_t {
  a_b_en  = 0,
  a_b     = 1,
  dir_en  = 2
};

enum input_type_t {
  pwm  = 0,
  adc  = 1,
  sbus = 2
};

typedef struct {
    short a;
    short b;
    short en;
    
} motor_pins_t;

typedef struct {
    motor_drive_mode_t mode;
    bool inverted;
    input_type_t input_type;
    int input_channel;
    
} motor_config_t;

#endif