#pragma once

#include <sbus_input.h>
#include <ps_input.h>
#include <adc_input.h>
#include <pwm_input.h>
#include <enow_input.h>

#include <motor.h>
#include <pwm_output.h>
#include <lego_servo.h>

#include <inputs_queue.h>

static global_config_t global_config;

void on_esp_now_received(const unsigned short* channels, int channels_count) {

  // for (auto input_config : global_config["esp_now"]) {
  //   if (input_config.out_type == motor) {
  //     outputs[input_config.out_channel] = map(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  //   }
  // }

  // SBUS Output
  for (auto i=0; i<channels_count; i++) {
     //sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
  }

  sbus_tx.data(sbus_data);
  sbus_tx.Write();

  // PWM Output
  pwm_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(sbus_data.ch, channels_count);
}

void driver_init() 
{
  adc_init();
  pwm_in_init();
  
  sbus_init();

  ps_init();
  //enow_init();

  motors_init();
  servos_init();
  servos_start();
  lego_servos_init();

  log_i("Initialization...\tDONE");
}

inline bool read_pwm(const short index, int16_t outputs[]) 
{ 
  if (pwm_inputs_count <= index) 
  {
    return false;
  }

  // TODO: Update this
  auto pwm_index = index; // motors[index].input_channel;
  auto pwm_value = input_pwm[pwm_index].Result();
  auto pwm_detected = pwm_value > INPUT_PWM_ZERO;

  //log_d("PWM IN %d: %d", index, pwm_value);

  if (pwm_detected)
  {
    outputs[index] = map(constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  }

  return pwm_detected;
}

void trace_values(const uint16_t outputs[], const uint8_t count)
{
  char trace[100];
  for (auto i = 0; i<count; ++i) {
    sprintf(trace, "%s\t%d", trace, outputs[i]);
  }
  log_d("%s", trace);
}

void driver_loop()
{
  int16_t outputs[motors_count];
  int16_t outputs_servo[servos_count];
  int16_t outputs_servo_lego[lego_servos_count];

  /* SBUS */
  if (sbus_rx.Read()) {
    sbus_data = sbus_rx.data();

    for (auto input_config : global_config["sbus"]) 
    {
      auto sbus_value = constrain(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX);
      switch (input_config.out_type)
      {
        case motor:
          outputs[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
          break;
        case servo:
          outputs_servo[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, SERVO_LOW, SERVO_HIGH);
          break;
        case servo_lego:
          outputs_servo_lego[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, LEGO_SERVO_LOW, LEGO_SERVO_HIGH);
          break;              
      }
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    write_motors(outputs, motors_count);
    pwm_write(outputs_servo, servos_count);
    delay(70);
    lego_servos_write(outputs_servo_lego, lego_servos_count);

    //trace_values(outputs_servo, servos_count);

    return;
  }

  /* PWM */
  for (auto i=0; i<motors_count; i++)
  {
    if (read_pwm(i, outputs) || adc_read<(-MOTOR_DUTY_CYCLE), (MOTOR_DUTY_CYCLE)>(i, outputs[i]))
    {
      write_motor(i, outputs[i]);
    }
    else
    {
      write_motor(i, 0);
    }
  }

  for (auto i = 0; i < lego_servos_count; i++)
  {
    // if (read_pwm(i, outputs_servo_lego))
    // {
    //   write_motor(i, outputs[i]);
    //   continue;
    // }
    // if (read_adc(i, outputs))
    // {
    //   write_motor(i, outputs[i]);
    //   continue;
    // }
    lego_servo_write(lego_servos[i], 0);
  }

  delay(50);
}