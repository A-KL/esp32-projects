#pragma once

inline bool near_zero(const int value) {
  return (abs(value) < 30);
}

void init_motors_a_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.en, pins.en_channel);
}

void init_motors_a_b_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);
  pinMode(pins.b, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);
  ledcAttachPin(pins.en, pins.en_channel);
}

void init_motors_a_b(const motor_pins_t& pins)
{
  ledcSetup(pins.a_channel, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(pins.b_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.a, pins.a_channel);
  ledcAttachPin(pins.b, pins.b_channel);
}

void motor_init(const motor_config_t& config)
{
     switch (config.mode)
     {
      case a_b_en:
          init_motors_a_b_en(config.pins);
          break;

      case a_b:
          init_motors_a_b(config.pins);
          break;

      case dir_en:
          init_motors_a_en(config.pins);
          break;
     
      default:
          break;
     }
}

void motors_init() 
{
  for (auto i=0; i<motors_count; ++i) {
      motor_init(motors[i]);
  }
}

void motor_run_a_b(const byte channel_a, const byte channel_b, const int speed) 
{
  if (near_zero(speed)) {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, 0);
  }
  else if (speed > 0) {
    ledcWrite(channel_a, abs(speed));
    ledcWrite(channel_b, 0);
  }
  else {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, abs(speed));
  }
}

void motor_run_a_en(const byte pin_a, const byte channel_en, const int speed) 
{
  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    ledcWrite(channel_en, abs(speed));
  }
}

void motor_run_a_b_en(const byte pin_a, const byte pin_b, const byte channel_en, const int speed) 
{
  if (near_zero(speed)) {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, LOW);

    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    digitalWrite(pin_b, speed > 0 ? LOW : HIGH);

    ledcWrite(channel_en, abs(speed));
  }
}

void motor_run(const motor_pins_t& pins, const motor_config_t& config, const int speed)
{
    switch (config.mode)
    {
      case a_b_en:
        motor_run_a_b_en(pins.a, pins.b, pins.en_channel, speed);
        break;

      case a_b:
        motor_run_a_b(pins.a_channel, pins.b_channel, speed);
        break;

      case dir_en:
        motor_run_a_en(pins.a, pins.en_channel, speed);
        break;
      
      default:
        break;
    }
}