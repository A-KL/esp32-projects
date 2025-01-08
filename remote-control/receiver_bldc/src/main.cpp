#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <driver.h>
#include <driver_strategy_t.h>
#include <motor_output.h>

#include <ps_input.h>

static const int16_t Power_Min = -255;
static const int16_t Power_Max = 255;
static const int16_t dead_zone = 18;

static driver_strategy_t<int16_t> left_value(Power_Min, Power_Max, 0);
static driver_strategy_t<int16_t> right_value(Power_Min, Power_Max, 0);

static inline int16_t filter_dead_zone(int16_t value)
{
  return (abs(value) < dead_zone) ? 0 : value;
}

static inline void write(int16_t power, int16_t steer)
{
  static int16_t left_speed = constrain(power - steer, Power_Min, Power_Max);
  static int16_t right_speed = constrain(power + steer, Power_Min, Power_Max);

  left_value.write(left_speed);
  right_value.write(right_speed);
}

static void on_ps3_input(int8_t lx, int8_t ly, int8_t rx, int8_t ry, int8_t l2, int8_t r2) 
{
  auto power = r2 - l2;
  auto steer = - lx;

  power = filter_dead_zone(power);
  steer = filter_dead_zone(steer);

  write(power, steer);
}

void setup() {
  Serial.begin(115200);

  delay(3000);

  ps_attach(on_ps3_input);
  ps_init();

  motors_init();

  // wifi_init();
  // now_init();
}

void loop() {
  auto speed_left = left_value.read();
  auto speed_right = right_value.read();

  log_d("Speed:\t%d\t%d\t(%d)", speed_left, speed_right, millis());
  delay(50);
}