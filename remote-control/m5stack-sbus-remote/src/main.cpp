#include <sbus.h>

#include <lv_tft_espi.h>
#include <lv_driver.h>
#include <lv_ui.h>

#include "ui/lv_screen.h"

#include "rf24_radio.h"
#include "now.h"
#include "controller.h"

#include "motor.h"
#include "buttons.h"

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int ch_count = 8;

const int ch_min_value = 200;
const int ch_max_value = 1800;
const int ch_max_count = 8;

inline int16_t percentage(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value) {
  return map(value, min, max, 0, 100);
}

inline int16_t speed(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value) {
  return map(value, min, max, -255, 255);
}

void on_esp_now_disconnected() {
    esp_now_values.clearValues();
}

void on_esp_now_message_received(const data_message_t& data) {
    for (uint8_t i = 0; i < ch_count; i++) {
      esp_now_values.setValue(i, data.channels[i].value);
    }
  // auto grab = map(data.channels[0].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);
  // auto left = map(data.channels[3].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 180, 0);
  // auto right = map(data.channels[5].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 10, 180);

  // driver.SetServoAngle(2, left);
  // driver.SetServoAngle(1, right);
  // driver.SetServoAngle(4, grab);

  // log_w("Commands: %d\t %d", left, right);
}

void hal_buttons_click(int8_t i)
{
  lv_ui_change_tab(i);
}

void setup()
{
  //Wire.begin();
  Serial.begin(115200);

  hal_buttons_init();

  lv_init();
  lv_driver_init();
  lv_ui_init();

  now_init();
  setupRadio();
  sbus_rx.Begin(16, 17);
  controller_init();

  // I2C
  hal_i2c_init();
}

void loop()
{
  static int16_t left_speed = 0;
  static int16_t right_speed = 0;
  static int16_t inputs[8] = { 0 };

  hal_buttons_loop();
  now_loop();
  lv_lcd_loop();

  // Game controller
  if (controller_loop(inputs))
  {
    auto power = inputs[0] - inputs[1];
    auto steer = - inputs[3];

    if (abs(power) < dead_zone) {
      power = 0;
    }

    if (abs(steer) < dead_zone) {
      steer = 0;
    }

    left_speed = constrain(power - steer, -255, 255);
    right_speed = constrain(power + steer, -255, 255);
  }

  // SBUS 
  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    for (uint8_t i = 0; i < ch_max_count; i++)
    {
      sbus_values.setValue(i, percentage(sbus_data[i]));
      //inputs[i] = sbus_data[i];
    }

    left_speed = map(sbus_data[2], ch_min_value, ch_max_value, -255, 255);
    right_speed = map(sbus_data[1], ch_min_value, ch_max_value, 255, -255);
  }
  else
  {
    sbus_values.clearValues();
  }

  if (RF24_IsReceived())
  {
    auto ch_left = received.channels[2].value;
    auto ch_right = received.channels[4].value;

    nrf42_values.setValue(0, ch_left);
    nrf42_values.setValue(1, ch_right);

    left_speed = map(ch_left, ch_min_value, ch_max_value, 255, -255);
    right_speed = map(ch_right, ch_min_value, ch_max_value, 255, -255);
  }
  else
  {
    nrf42_values.clearValues();
  }

  // Power
  if (ina219_output_connected && ina219_input_connected)
  {
    auto shunt_voltage = ina219_output.getShuntVoltage_mV();
    auto bus_voltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto load_voltage = bus_voltage + (shunt_voltage / 1000);

    // power_values.setText(0, "%.2f v", bus_voltage);
    // power_values.setText(1, "%.2f mA", current_mA);
    // power_values.setText(2, "%.2f mW", power_mW);
  }
  else
  {
    power_values.clearValues();
  }

  // Encoder
  for (int8_t i = 0; i < 4; i++) {
    if (motor_driver_connected) {  
      encoder_values.setValue(i, motor_read_encoder(i));
    } else {
      encoder_values.clearValue(i);
    }
  }

  // Motor
  motor_loop(left_speed, right_speed);

//   // Acc
//   int16_t temp = 0;
//   float pitch = 0;
//   float roll = 0;
//   float yaw = 0;

//   imu.getTempAdc(&temp);
//   imu.getAhrsData(&pitch, &roll, &yaw);

//   log_d("MPU6886 Temp: %d", temp);
//   log_d("MPU6886 pitch: %f roll: %f yaw: %f", pitch, roll, yaw);

//  // power_values.setText(3, "temp %.2f", temp);
//   power_values.setText(3, "p %.2f", pitch);
//   power_values.setText(4, "r %.2f", roll);
//   power_values.setText(5, "y %.2f", yaw);
}