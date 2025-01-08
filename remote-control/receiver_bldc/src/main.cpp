#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <driver.h>
#include <driver_strategy_t.h>
#include <motor_output.h>

#include <ps_input.h>

driver_strategy_t<int16_t> left_value(-255, 255, 0);
driver_strategy_t<int16_t> right_value(-255, 255, 0);

static void on_ps3_input(int8_t lx, int8_t ly, int8_t rx, int8_t ry, int8_t l2, int8_t r2) 
{
    const static int dead_zone = 20;
    
    auto power = r2 - l2;
    auto steer = - lx;

    if (abs(power) < dead_zone) {
      power = 0;
    }
    if (abs(steer) < dead_zone) {
      steer = 0;
    }

    static int16_t left_speed = constrain(power - steer, -255, 255);
    static int16_t right_speed = constrain(power + steer, -255, 255);

  // left_value.write();
  // right_value.write();
}

void setup() {
  Serial.begin(115200);

  delay(5000);

  ps_attach(on_ps3_input);
  ps_init();

  left_value.write(-255);
  right_value.write(255);

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