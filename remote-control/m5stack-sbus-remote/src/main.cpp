#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include <sbus.h>

#include "M5Unified.h"
#include "MPU6886.h"

#include "lv_tft_espi.h"
#include "lv_common.h"

#include "ui/lv_widgets.h"
#include <lv_ui.h>
#include "ui/main_screen.h"

#include "rf24_radio.h"
#include "now.h"
#include "controller.h"

#include "motor.h"

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

bool ina219_output_connected = false;
bool ina219_input_connected = false;

MPU6886 imu;

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

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
    for (int8_t i = 0; i < 5; i++) {
      esp_now_values.setText(i, "ch%d --", i);
    }
}

void on_esp_now_message_received(const data_message_t& data) {
    for (int8_t i = 0; i < 5; i++) {
      esp_now_values.setText(i, "ch%d %d%", i, data.channels[i].value);
    }
  // auto grab = map(data.channels[0].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);
  // auto left = map(data.channels[3].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 180, 0);
  // auto right = map(data.channels[5].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 10, 180);

  // driver.SetServoAngle(2, left);
  // driver.SetServoAngle(1, right);
  // driver.SetServoAngle(4, grab);

  // log_w("Commands: %d\t %d", left, right);
}

void buttons_loop()
{
  if (M5.BtnA.wasPressed()) {
      Serial.println("BtnA Pressed");
      lv_ui_change_tab(0);
  } else if (M5.BtnB.wasPressed()) {
      Serial.println("BtnB Pressed");
      lv_ui_change_tab(1);
  } else if (M5.BtnC.wasPressed()) {
      Serial.println("BtnC Pressed");
      lv_ui_change_tab(2);
  }
}

void setup()
{
  Serial.begin(115200);

  lv_init();
  lv_lcd_common_init();
  lv_ui_init();

  auto cfg = M5.config();
  M5.begin(cfg);

  now_init();
  setupRadio();

  sbus_rx.Begin(16, 17);

  controller_init();

  if (!ina219_output.begin()) {
    log_i("Failed to find output INA219 chip");
    ina219_output_connected = false;
  }

  motor_init();

  //imu.Init();
}

void loop()
{
  static int16_t left_speed = 0;
  static int16_t right_speed = 0;

  M5.update();

  buttons_loop();

  now_loop();

  lv_lcd_loop();

  if (controller_loop(left_speed, right_speed))
  {
    ps_values.setText(0, "p %d", left_speed);
    ps_values.setText(1, "s %d", right_speed);
  }
  else
  {
    ps_values.setText(0, "ls ---");
    ps_values.setText(1, "rs ---");
  }
  return;
  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    for (int8_t i = 0; i < ch_max_count; i++)
    {
      sbus_values.setText(i, "ch%d %d%", i, percentage(sbus_data[i]));

      // Serial.print(sbus_data[i]);
      // Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    //log_d("%d\t%d", sbus_rx.lost_frame(), sbus_rx.failsafe());

    left_speed = map(sbus_data[2], ch_min_value, ch_max_value, -255, 255);
    right_speed = map(sbus_data[1], ch_min_value, ch_max_value, 255, -255);
  }
  else
  {
    for (int8_t i = 0; i < ch_max_count; i++) {
      sbus_values.setText(i, "ch%d ---", i);
    }
  }

  if (RF24_IsReceived())
  {
    auto ch_left = received.channels[2].value;
    auto ch_right = received.channels[4].value;

    left_speed = map(ch_left, ch_min_value, ch_max_value, 255, -255);
    right_speed = map(ch_right, ch_min_value, ch_max_value, 255, -255);

    nrf42_values.setText(0, "ch0 %d", ch_left);
    nrf42_values.setText(1, "ch1 %d", ch_right);
  }
  else
  {
    nrf42_values.setText(0, "ch0 ---");
    nrf42_values.setText(1, "ch1 ---");
  }

  // Power
  if (ina219_output_connected && ina219_input_connected)
  {
    auto shunt_voltage = ina219_output.getShuntVoltage_mV();
    auto bus_voltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto load_voltage = bus_voltage + (shunt_voltage / 1000);

    power_values.setText(0, "%.2f v", bus_voltage);
    power_values.setText(1, "%.2f mA", current_mA);
    power_values.setText(2, "%.2f mW", power_mW);
  }
  else
  {
    power_values.setText(0, "--- V");
    power_values.setText(1, "--- mA");
    power_values.setText(2, "--- mW");
  }

  // Encoder
  for (int8_t i = 0; i < 4; i++)
  {
    encoder_values.setText(i, "ch%d %d", i, motor_read_encoder(i));
  }

  // Motor
  if (motor_loop(left_speed, right_speed))
  {
    motors_values.setText(0, "l %d", left_speed);
    motors_values.setText(1, "r %d", right_speed);
  }
  else
  {
    motors_values.setText(0, "l ---");
    motors_values.setText(1, "r ---");
  }

  

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