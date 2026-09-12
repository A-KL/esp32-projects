#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include "lvgl_port.h"

#include "lcd_bl_bsp/lcd_bl_pwm_bsp.h"

#include "imu_bsp/qmi_bsp.h"

void lv_lcd_init()
{
  lvgl_port_init();
  lcd_bl_pwm_bsp_init(LCD_PWM_MODE_100);

  gpio_config_t gpio_conf = {};
  gpio_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_conf.mode = GPIO_MODE_OUTPUT;
  gpio_conf.pin_bit_mask = 0x1ULL << GPIO_NUM_8;
  gpio_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  gpio_conf.pull_up_en = GPIO_PULLUP_ENABLE;

  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_config(&gpio_conf));

  gpio_set_level(GPIO_NUM_8,true);

  // auto id = master_imu_read_id();
  // log_e("IMU_ID:%02x", id);
  // log_e("IMU INIT: %s", (id == 0x05) ? "YES" : "NO");

  // if (imu_init()) {
  // //   imu_get();
  // }

  qmi_init();
  // i2c_qmi_get();
}