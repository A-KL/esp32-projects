#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include "user_config.h"
#include "lvgl_port.h"

#include "lcd_bl_bsp/lcd_bl_pwm_bsp.h"

void lv_lcd_init()
{

  lvgl_port_init();
  lcd_bl_pwm_bsp_init(LCD_PWM_MODE_125);
}