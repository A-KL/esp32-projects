#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <storage.h>
#include <inputs_queue.h>
#include <network.h>
#include <web_server.h>
#include <lcd.h>
//#include <scheduler.h>
//#include <button_input.h>

#include <sbus_input.h>
#include <ps_input.h>
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

static global_config_t global_config;

void setup() {
  Serial.begin(115200);
  Serial.println(HOSTNAME);
  sleep(2);

  lcd_init();
  storage_init();
  settings_load(global_config);
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
  
  // wifi_init(HOSTNAME);
  // server_init();
}

template<short TMin, short TMax>
void write_mapped_outputs(const int16_t* inputs, const uint8_t inputs_count, const char* config_name)
{
    if (inputs_count == 0) {
      return;
    }

    static int16_t outputs_motors[motors_count];
    static int16_t outputs_servo[servos_count];

    // Motors
    settings_map_inputs(global_config, config_name, inputs, motor, outputs_motors, motors_count);
    write_motors<TMin, TMax>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    settings_map_inputs(global_config, config_name, inputs, servo, outputs_servo, servos_count);
    servos_write<TMin, TMax>(outputs_servo, servos_count);
}

void loop() {
  static int16_t inputs[32];

  static int16_t outputs_motors[motors_count];
  static int16_t outputs_servo[servos_count];

  // SBUS
  if (sbus_receive(inputs) > 0) 
  {
    // Motors
    settings_map_inputs(global_config, "sbus", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    settings_map_inputs(global_config, "sbus", inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, servos_count);
  }
  else if (enow_receive(inputs) > 0)
  {
    // Motors
    settings_map_inputs(global_config, "esp_now", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    settings_map_inputs(global_config, "esp_now", inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_servo, servos_count);
  }
  else if (ps_receive(inputs) > 0)
  {
    // Motors
    settings_map_inputs(global_config, "ps3", inputs, motor, outputs_motors, motors_count);
    write_motors<-INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    settings_map_inputs(global_config, "ps3", inputs, servo, outputs_servo, servos_count);
    servos_write<-INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_servo, servos_count);
  }
  else if (adc_receive(inputs) > 0)
  {
    // Motors
    settings_map_inputs(global_config, "adc", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_ADC_MIN, INPUT_ADC_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    settings_map_inputs(global_config, "adc", inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ADC_MIN, INPUT_ADC_MAX>(outputs_servo, servos_count);
  }
  else // No input
  {
    // Motors
    for (size_t i = 0; i < motors_count; i++) {
      outputs_motors[i] = 0;
    }
    write_motors(outputs_motors, motors_count);

    // Servos
    servos_attach(false);
  }

  delay(50);
  
  adc_loop();

  // server_loop();
  // scheduler_loop();
  // button_input_update(switch_input);
}