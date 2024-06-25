#pragma once

bfs::SbusRx sbus_rx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

void on_esp_now(const channel_t* channels, int channels_count) {

  // for (auto input_config : global_config["esp_now"]) {
  //   if (input_config.out_type == motor) {
  //     outputs[input_config.out_channel] = map(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  //   }
  // }

  // SBUS Output
  for (auto i=0; i<channels_count; i++) {
      sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
  }

  sbus_tx.data(sbus_data);
  sbus_tx.Write();

  // PWM Output
  pwm_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(sbus_data.ch, channels_count);
}

void driver_init() 
{
  for (auto i = 0; i < adc_inputs_count; i++) {
      pinMode(adc_input_pins[i], INPUT);
  }

  pwm_in_init();
  
  sbus_rx.Begin();
  sbus_tx.Begin();

  motors_init();
  servos_init();
  servos_start();
  lego_servos_init();
}

inline bool read_pwm(const short index, int outputs[]) 
{ 
  // TODO: Update this
  auto pwm_index = index; // motors[index].input_channel;
  auto pwm_value = input_pwm[pwm_index].Result();

  auto pwm_detected = pwm_value > INPUT_PWM_ZERO;

  if (pwm_detected)
  {
    outputs[index] = map(constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  }

  return pwm_detected;
}

inline bool read_adc(const short index, int outputs[]) 
{
  outputs[index] = 0;
  
  //return true;

 // TODO: Update this
  auto adc_index = index;// motors_config[index].input_channel;
  auto adc_value = analogRead(adc_input_pins[adc_index]);

  log_d("ADC IN %d: %d", index, adc_value);

  outputs[index] = map(adc_value, INPUT_ADC_MIN, INPUT_ADC_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  
  return true;
}

void write_motor(short index, int output)
{
  motor_run(motors[index].pins, motors[index], output);
  
  log_d("MOTOR OUT %d: %d", index, output);
}

void write_motors(int outputs[], short count)
{
  for (short i = 0; i<count; ++i) {
    write_motor(i, outputs[i]);
  }
}

inline void write_lego_servo(short index, int output)
{
  lego_servo_write(lego_servos[index], output);

  log_d("LEGO SERVO OUT %d: %d", index, output);
}

inline void write_lego_servos(int outputs[], short count)
{
  for (short i = 0; i<count; ++i) {
    write_lego_servo(i, outputs[i]);
  }
}

// template<const int TMin, const int TMax>
// void write_motors(short index, int outputs[])
// {
//   auto mapped_value = map(outputs[index], TMin, TMax, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);

//   run_motor(motors[index].pins, motors[index], mapped_value);
  
//   log_d("MOTOR OUT %d: %d", index, mapped_value);
// }

void driver_loop()
{
  int outputs[motors_count];
  short outputs_servo[servos_count];
  int outputs_servo_lego[lego_servos_count];

  /* SBUS */
  if (sbus_rx.Read()) {
    sbus_data = sbus_rx.data();

    for (auto input_config : global_config["sbus"]) {
      if (input_config.out_type == motor) {
         outputs[input_config.out_channel] = map(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
      }
      if (input_config.out_type == servo) {
          outputs_servo[input_config.out_channel] = map(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX, SERVO_LOW, SERVO_HIGH);
      }
      if (input_config.out_type == servo_lego) {
          outputs_servo_lego[input_config.out_channel] = map(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX, LEGO_SERVO_LOW, LEGO_SERVO_HIGH);
      }
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    write_motors(outputs, motors_count);
    pwm_write(outputs_servo, servos_count);
    write_lego_servos(outputs_servo_lego, lego_servos_count);

    return;
  }

  /* PWM */
  for (short i=0; i<motors_count; i++)
  {
    if (read_pwm(i, outputs))
    {
      write_motor(i, outputs[i]);
      continue;
    }
    if (read_adc(i, outputs))
    {
      write_motor(i, outputs[i]);
      continue;
    }
  }

  for (auto i = 0; i < lego_servos_count; i++)
  {
    /* code */
  }

  delay(50);
}