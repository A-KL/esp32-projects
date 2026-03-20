#include <Arduino.h>

#ifndef DEVICE_ID
#error "Device-ID is not defined!"
#endif

#ifndef CONTROLS_NAME
#define CONTROLS_NAME config_json
#endif
#define CONTROLS_FILE <CONTROLS_NAME.h>
#include CONTROLS_FILE

#define Q(x) #x
#define QUOTE(x) Q(x)

#define HOSTNAME_PREFIX esp32
#define HOSTNAME QUOTE(HOSTNAME_PREFIX-DEVICE_ID)

#include <inputs_queue.h>
#include <network.h>
#include <web_server.h>
#include <lcd.h>
//#include <scheduler.h>
//#include <button_input.h>

#include <sbus_input.h>
#include <ps3_input.h>
#include <adc_input.h>
#include <pwm_input.h>
#include <enow_input.h>
#include <xbox_input.h>

#include <motor_output.h>
#include <servo_output.h>
#include <lego_servo_output.h>

// static void on_switch_input(short input) {
//     log_w("Button value: %d", input);
//     short data[16];

//     //send_sbus_data(data, 16);
// }
//static button_input_t switch_input { switch_input_button, LOW, 2000, 0, 3, 0,  &on_switch_input};

void setup() {
  Serial.begin(115200);
  Serial.println(HOSTNAME);
  sleep(2);

  lcd_init();
  config_init();
  init_wifi();

  //button_input_init(switch_input);
  //scheduler_add(300, [](){ send_sbus_data(sbus_data.ch, sbus_data.NUM_CH); });

  adc_init();
  pwm_in_init();
  sbus_init();
  ps_init();
  xbox_init();
  enow_init();

  motors_init();
  servos_init();

  log_i("Initialization...\tDONE");
  
  // server_init();
}

// template<short TMin, short TMax>
// void write_mapped_outputs(const int16_t* inputs, const uint8_t inputs_count, const char* config_name)
// {
//     if (inputs_count == 0) {
//       return;
//     }

//     static int16_t outputs_motors[motors_count];
//     static int16_t outputs_servo[servos_count];

//     // Motors
//     controls_map_inputs(config_name, inputs, dc, outputs_motors, motors_count);
//     write_motors<TMin, TMax>(outputs_motors, motors_count);

//     // Servos
//     servos_attach(true, servos_count);
//     controls_map_inputs(config_name, inputs, servo, outputs_servo, servos_count);
//     servos_write<TMin, TMax>(outputs_servo, servos_count);
// }

void loop() {
  static int16_t inputs[32];

  static int16_t outputs_motors[motors_count];
  static int16_t outputs_servo[servos_count];
  static int16_t outputs_lego_servo[lego_servos_count];

  // SBUS
  if (sbus_receive(inputs) > 0) 
  {
    // Motors
    controls_map_inputs(sbus, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(sbus, inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, servos_count);

    // Lego Servo
    controls_map_inputs(sbus, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    lego_servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else if (enow_receive(inputs) > 0)
  {
    // Motors
    controls_map_inputs(esp_now, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(esp_now, inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_servo, servos_count);

    // Lego Servo
    controls_map_inputs(esp_now, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    lego_servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else if (ps_receive(inputs) > 0)
  {
    // Motors
    controls_map_inputs(ps3, inputs, dc, outputs_motors, motors_count);
    write_motors<-INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(ps3, inputs, servo, outputs_servo, servos_count);
    servos_write<-INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_servo, servos_count);

    // Lego Servo
    controls_map_inputs(ps3, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    lego_servos_write<INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_lego_servo, lego_servos_count);
  } 
  else if (pwm_receive(inputs))
  {
    // Motors
    config_map_inputs(pwm, inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(pwm, inputs, servo, outputs_servo, servos_count);
    servos_write<-INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_servo, servos_count);

    // Lego Servo
    config_map_inputs(pwm, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    lego_servos_write<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else if (adc_receive(inputs) > 0)
  {
    // Motors
    controls_map_inputs(adc, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_ADC_MIN, INPUT_ADC_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(adc, inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ADC_MIN, INPUT_ADC_MAX>(outputs_servo, servos_count);

    // Lego Servo
    controls_map_inputs(adc, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    lego_servos_write<INPUT_ADC_MIN, INPUT_ADC_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else // No input
  {
    // Motors
    for (size_t i = 0; i < motors_count; i++) {
      outputs_motors[i] = 0;
    }
    // write_motors(outputs_motors, motors_count);

    motors_stop();

    // Servos
    servos_attach(false);

    // Lego Servo
    lego_servos_stop();
  }

  delay(50);
  
  adc_loop();

  // server_loop();
  // scheduler_loop();
  // button_input_update(switch_input);
}