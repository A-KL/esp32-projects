#pragma once

#include <esp_log.h>

#include <Adafruit_INA219.h>
#include <MPU6886.h>
#include <lego_plus_driver.h>
#include <GoPlus2.h>

#include "ui/lv_screen.h"

static bool motor_driver_connected = false;
static bool motor_driver_v2_connected = false;

static bool ina219_output_connected = false;
static bool ina219_input_connected = false;

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

MPU6886 imu;

GoPlus2 goPlus;

void motor_init()
{
  auto version = readVersion();
  motor_driver_connected = version != 0;
  log_i("Motor Driver ver: %d\r\n", version);

  goPlus.begin();
  motor_driver_v2_connected = goPlus.hub1_a_read_value(HUB1_R_ADDR) > 0;
  log_i("Motor Driver V2 available: %d", motor_driver_v2_connected);
}

int32_t motor_read_encoder(uint8_t index)
{
    return motor_driver_connected ? readEncoder(index) : 0l;
}

bool motor_loop(int16_t left_speed, int16_t right_speed)
{
  if (motor_driver_connected)
  {
    MotorRun(1, -left_speed);
    MotorRun(2, -left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);

    motor_values.setValue(0, -left_speed);
    motor_values.setValue(1, -left_speed);
    motor_values.setValue(2, right_speed);
    motor_values.setValue(3, right_speed);
  }
  else if (motor_driver_v2_connected)
  {
    goPlus.Motor_write_speed(MOTOR_NUM0, -left_speed/2);
    goPlus.Motor_write_speed(MOTOR_NUM1, -right_speed/2);

    motor_values.setValue(0, -left_speed/2);
    motor_values.setValue(1, -right_speed/2);
    motor_values.setValue(2, 0);
    motor_values.setValue(3, 0);
    // motors_values.setText(0, "l %d", -left_speed/2);
    // motors_values.setText(1, "r %d", -right_speed/2);

    goPlus.Servo_write_angle(SERVO_NUM0,  map(left_speed, -255, 255, 0, 180));
    goPlus.Servo_write_angle(SERVO_NUM1,  map(right_speed, -255, 255, 0, 180));
  }
  else
  {
    motor_values.clearValues();
  }

  return motor_driver_connected || motor_driver_v2_connected;
}

void hal_i2c_init()
{
  motor_init();

  imu.Init();

  if (!ina219_output.begin()) {
    log_i("Failed to find output INA219 chip");
    ina219_output_connected = false;
  }

  if (!ina219_input.begin()) {
    log_i("Failed to find input INA219 chip");
    ina219_input_connected = false;
  }
}