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

static motor_driver_t motor_left(25, 33, 15000);
static motor_driver_t motor_right(26, 27, 15000);

static driver_strategy_t<int16_t> left_value(Power_Min, Power_Max, 0);
static driver_strategy_t<int16_t> right_value(Power_Min, Power_Max, 0);

static inline int16_t filter_dead_zone(const int16_t value)
{
  return (abs(value) < dead_zone) ? 0 : value;
}

static inline void write(const int16_t power, const int16_t steer)
{
  int16_t left_speed = constrain(power - steer, Power_Min, Power_Max);
  int16_t right_speed = constrain(power + steer, Power_Min, Power_Max);

  left_value.write(left_speed);
  right_value.write(right_speed);
}

static void on_ps3_lost()
{
  write(0, 0);
}

static void on_ps3_input(int16_t lx, int16_t ly, int16_t rx, int16_t ry, int16_t l2, int16_t r2) 
{
  auto power = r2 - l2;
  auto steer = - lx;

  power = filter_dead_zone(power);
  steer = filter_dead_zone(steer);

  // left_value.write(l2);
  // right_value.write(r2);

  write(power, steer);
}

void setup() {
  Serial.begin(115200);

  delay(3000);

  ps_attach(on_ps3_input, on_ps3_lost);
  ps_init();

  motors_init();

  motor_left.begin().add_brake(12);

  // wifi_init();
  // now_init();
}

void loop() {
  auto left = left_value.read();
  auto right = right_value.read();

  log_d("Speed:\t%d\t%d", left, right);
  delay(50);

  motor_write_a_en(motor_dir[0], motor_pwm_ch[0], left);
  motor_write_a_en(motor_dir[1], motor_pwm_ch[1], right);
}