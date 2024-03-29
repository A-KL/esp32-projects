//#include <M5Stack.h>
#include <Wire.h>
#include <sbus.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include "NotoSansBold15.h"
#include "lego_plus_driver.h"
#include "radio.h"
#include "gui.h"

#include <esp32_now.h>

#if defined(PS3)
  #include <Ps3Controller.h>
  #define INIT_CONTROLLER Ps3.begin("b8:27:eb:df:b3:ff")
  #define Ps3.isConnected()
#elif defined(PS4)
  #include <PS4Controller.h>
  #define INIT_CONTROLLER PS4.begin("b8:27:eb:df:b3:ff")
  #define PS4.isConnected()
#else
  #include <XboxSeriesXControllerESP32_asukiaaa.hpp>

  XboxSeriesXControllerESP32_asukiaaa::Core
  xboxController("4c:3b:df:a5:1c:42");

  #define INIT_CONTROLLER xboxController.begin()
  #define CONTROLLER_CONNECTED xboxController.isConnected()
#endif

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

bool ina219_output_connected = false;
bool ina219_input_connected = false;
bool motor_driver_connected = false;

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int ch_min_value = 200;
const int ch_max_value = 1800;
const int ch_max_count = 8;

int16_t percentage(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value)
{
  return map(value, min, max, 0, 100);
}

int16_t speed(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value)
{
  return map(value, min, max, -255, 255);
}

void on_esp_now_disconnected() {
    for (int8_t i = 0; i < 6; i++) {
        //esp_now_values.setText(i, "ch%d --", i);
    }
}

void on_esp_now_message_received(const data_message_t& data) {
    for (int8_t i = 0; i < 6; i++) {
     // esp_now_values.setText(i, "ch%d %d%", i, data.channels[i].value);
  }
  // auto grab = map(data.channels[0].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);
  // auto left = map(data.channels[3].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 180, 0);
  // auto right = map(data.channels[5].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 10, 180);

  // driver.SetServoAngle(2, left);
  // driver.SetServoAngle(1, right);
  // driver.SetServoAngle(4, grab);

  // log_w("Commands: %d\t %d", left, right);
}

void setup() {

  Wire.begin();
  Serial.begin(115200);

  xboxController.begin();

  // if (INIT_CONTROLLER)
  // {
  //   Serial.println("Failed to start Controller Host");
  // }
  now_init();
  setupRadio();

  if (!ina219_output.begin()) {
    Serial.println("Failed to find output INA219 chip");
    ina219_output_connected = false;
  }

  if (!ina219_input.begin()) {
    Serial.println("Failed to find input INA219 chip");
    ina219_input_connected = false;
  }

  auto version = readVersion();
  motor_driver_connected = version != 0;
  Serial.printf("Motor Driver ver: %d\r\n", version);

  sbus_rx.Begin(16, 17);

  gui_init();
}

void loop() {
  auto left_speed = 0;
  auto right_speed = 0;

  xboxController.onLoop();
  now_loop();

  if (CONTROLLER_CONNECTED) {

    //PS3.setPlayer(1);

    // ps3_values.setText(0, "ls %d", PS4.data.analog.stick.ly);
    // ps3_values.setText(1, "rs %d", PS4.data.analog.stick.ry);

    // left_speed = map(PS4.data.analog.stick.ly, -128, 128, -255, 255);
    // right_speed = map(PS4.data.analog.stick.ry, 128, -128, 255, -255);
  }
  else
  {
    ps3_values.setText(0, "ls ---");
    ps3_values.setText(1, "rs ---");
  }

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    //spr.setTextColor(WHITE); 

    for (int8_t i = 0; i < ch_max_count; i++)
    {
      sbus_values.setText(i, "ch%d %d%", i, percentage(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());

    left_speed = map(sbus_data[2], ch_min_value, ch_max_value, -255, 255);
    right_speed = map(sbus_data[1], ch_min_value, ch_max_value, 255, -255);
  }
  else
  {
    for (int8_t i = 0; i < ch_max_count; i++) {
      sbus_values.setText(i, "ch%d ---", i);
    } 
  }

  if (isReceived()) 
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

  if (ina219_output_connected && ina219_input_connected) 
  {
    auto shunt_voltage = ina219_output.getShuntVoltage_mV();
    auto bus_voltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto load_voltage = bus_voltage + (shunt_voltage / 1000);

    power_values.setText(0, "%.2f V", bus_voltage);
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
    encoder_values.setText(i, "ch%d %d", i, motor_driver_connected ? readEncoder(i) : 0);
  }

  // Motor
  if (motor_driver_connected) 
  {
    MotorRun(1, -left_speed);
    MotorRun(2, -left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);

    motors_values.setText(0, "l %d", left_speed);
    motors_values.setText(1, "r %d", right_speed);
  }
  else
  {
    motors_values.setText(0, "l ---");
    motors_values.setText(1, "r ---");
  }

  // Update GUI

  sbus_values.render(spr);
  ps3_values.render(spr);
  nrf42_values.render(spr);
  encoder_values.render(spr);
  motors_values.render(spr);
  power_values.render(spr);
  esp_now_values.render(spr);
}