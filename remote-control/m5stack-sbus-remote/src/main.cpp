#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include "M5Unified.h"

#include "NotoSansBold15.h"
#include "gui.h"

#include "lego_plus_driver.h"
#include "GoPlus2.h"
#include "MPU6886.h"
#include "sound.h"

#include "rf24_radio.h"
#include "now.h"
#include <sbus.h>

#ifdef PS3
  #include <Ps3Controller.h>
  #define INIT_CONTROLLER Ps3.begin("b8:27:eb:df:b3:ff")
  #define CONTROLLER_CONNECTED Ps3.isConnected()
#elif PS4
  #include <PS4Controller.h>
  #define INIT_CONTROLLER PS4.begin("b8:27:eb:df:b3:ff")
  #define PS4.isConnected()
#elif XBOX
  #include <XboxSeriesXControllerESP32_asukiaaa.hpp>

  XboxSeriesXControllerESP32_asukiaaa::Core
  xboxController("4c:3b:df:a5:1c:42");

  #define INIT_CONTROLLER xboxController.begin()
  #define CONTROLLER_CONNECTED xboxController.isConnected()
#else
  #define INIT_CONTROLLER log_i("No controller")
  #define CONTROLLER_CONNECTED false
#endif

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

bool ina219_output_connected = false;
bool ina219_input_connected = false;
bool motor_driver_connected = false;
bool motor_driver_v2_connected = false;

MPU6886 imu;
GoPlus2 goPlus;
bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int ch_min_value = 200;
const int ch_max_value = 1800;
const int ch_max_count = 8;

// motor_a = wheel(y + x)
// motor_b = wheel(y - x)
inline float wheel(const float v, const float max_value = 255) {
    return max_value * constrain(v, -1, 1);
}

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

void setup() 
{
    auto cfg = M5.config();
    M5.begin(cfg);

  Wire.begin();
  Serial.begin(115200);

  imu.Init();
  sound_init();

  INIT_CONTROLLER;

  // if (INIT_CONTROLLER)
  // {
  //   log_i("Failed to start Controller Host");
  // }
  now_init();
  setupRadio();

  if (!ina219_output.begin()) {
    log_i("Failed to find output INA219 chip");
    ina219_output_connected = false;
  }

  if (!ina219_input.begin()) {
    log_i("Failed to find input INA219 chip");
    ina219_input_connected = false;
  }

  auto version = readVersion();
  motor_driver_connected = version != 0;
  log_i("Motor Driver ver: %d\r\n", version);

  goPlus.begin();
  motor_driver_v2_connected = goPlus.hub1_a_read_value(HUB1_R_ADDR) > 0;
  log_i("Motor Driver V2 available: %d", motor_driver_v2_connected);

  sbus_rx.Begin(16, 17);

  gui_init();
}

void loop() 
{
  auto left_speed = 0;
  auto right_speed = 0;

  M5.update();

  if (M5.BtnA.wasPressed()) {
      Serial.println("BtnA Pressed");
  }

  //xboxController.onLoop();
  now_loop();

  if (CONTROLLER_CONNECTED) 
  {
    Ps3.setPlayer(1);

    const static int dead_zone = 20;
    
    auto power = Ps3.data.analog.button.r2 - Ps3.data.analog.button.l2;
    auto steer = - Ps3.data.analog.stick.lx;

    if (abs(power) < dead_zone) {
      power = 0;
    }
    if (abs(steer) < dead_zone) {
      steer = 0;
    }

    ps_values.setText(0, "p %d", power);
    ps_values.setText(1, "s %d", steer);

    // motor_a = wheel(y + x)
    // motor_b = wheel(y - x)
    
    left_speed = constrain(power - steer, -255, 255); // constrain((Ps3.data.analog.stick.ry + Ps3.data.analog.stick.rx), -128, 128) / 128 * 255;
    right_speed = constrain(power + steer, -255, 255); // constrain((Ps3.data.analog.stick.ry - Ps3.data.analog.stick.rx), -128, 128) / 128 * 255;

    // if (abs(Ps3.data.analog.stick.ly) > dead_zone) {
    //   left_speed = map(Ps3.data.analog.stick.ly, -128, 128, -255, 255);
    // }

    // if (abs(Ps3.data.analog.stick.ry) > dead_zone) {
    //   right_speed = map(Ps3.data.analog.stick.ry, -128, 128, 255, -255);
    // }
  }
  else
  {
    ps_values.setText(0, "ls ---");
    ps_values.setText(1, "rs ---");
  }

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    for (int8_t i = 0; i < ch_max_count; i++)
    {
      sbus_values.setText(i, "ch%d %d%", i, percentage(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    log_d("%d\t%d", sbus_rx.lost_frame(), sbus_rx.failsafe());

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

  // Acc
  int16_t temp = 0;
  float pitch = 0;
  float roll = 0;
  float yaw = 0;

  imu.getTempAdc(&temp);
  imu.getAhrsData(&pitch, &roll, &yaw);

  log_d("MPU6886 Temp: %d", temp);
  log_d("MPU6886 pitch: %f roll: %f yaw: %f", pitch, roll, yaw);

 // power_values.setText(3, "temp %.2f", temp);
  power_values.setText(3, "p %.2f", pitch);
  power_values.setText(4, "r %.2f", roll);
  power_values.setText(5, "y %.2f", yaw);

  // Encoder
  for (int8_t i = 0; i < 4; i++) 
  {
    encoder_values.setText(i, "ch%d %d", i, motor_driver_connected ? readEncoder(i) : 0);
  }

  // Sound
  // if (left_speed > 0 && right_speed < 0) {
  //   sound_on(true);
  // }

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
  else if (motor_driver_v2_connected)
  {
    goPlus.Motor_write_speed(MOTOR_NUM0, -left_speed/2);
    goPlus.Motor_write_speed(MOTOR_NUM1, -right_speed/2);

    motors_values.setText(0, "l %d", -left_speed/2);
    motors_values.setText(1, "r %d", -right_speed/2);

    goPlus.Servo_write_angle(SERVO_NUM0,  map(left_speed, -255, 255, 0, 180));
    goPlus.Servo_write_angle(SERVO_NUM1,  map(right_speed, -255, 255, 0, 180)); 
  }
  else 
  {
    motors_values.setText(0, "l ---");
    motors_values.setText(1, "r ---");

    //sound_on(false);
  }

  //Speaker.update();

  // Update GUI
  gui_render();
}