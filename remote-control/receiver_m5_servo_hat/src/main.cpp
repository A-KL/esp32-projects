#include <Arduino.h>
#include <ps_input.h>
#include <M5GFX.h>
#include "Hat_8Servos.h"

M5GFX display;
M5Canvas canvas(&display);
Hat_8Servos drive;

static const int16_t Power_Min = 0;
static const int16_t Power_Max = 255;

static const int16_t dead_zone = 20;

static const int16_t Power_Channel_A = 0;
static const int16_t Power_Channel_B = 1;
static const int16_t Steering_Channel_A = 2;
static const int16_t Steering_Channel_B = 3;

// static inline int16_t filter_dead_zone(const int16_t value)
// {
//   return (abs(value) < dead_zone) ? 0 : value;
// }

static void on_ps3_lost()
{
  drive.setServoAngle(Steering_Channel_A, 90);
  drive.setServoAngle(Steering_Channel_B, 90);
  drive.setServoAngle(Power_Channel_A, 90);
  drive.setServoAngle(Power_Channel_B, 90);

  drive.enableServoPower(0);
}

static void on_ps3_input(int16_t lx, int16_t ly, int16_t rx, int16_t ry, int16_t l2, int16_t r2, int16_t triangle, int16_t cross) 
{
  auto power_angle_fw = map(r2, Power_Min, Power_Max, 0, 90);
  auto power_angle_bw = 90 - map(l2, Power_Min, Power_Max, 0, 90);

  auto power = power_angle_fw + power_angle_bw; 

  drive.setServoAngle(Power_Channel_A, power);
  drive.setServoAngle(Power_Channel_B, power);

  const auto half_range = (Power_Max+Power_Min)/2;

  auto steering = map(lx, -half_range, half_range, 0, 180);

  drive.setServoAngle(Steering_Channel_A, steering);
  drive.setServoAngle(Steering_Channel_B, steering);

  //log_d("Speed:\t%d\t%d\t%d\t%d", power_angle_fw, power_angle_bw, power, steering);
}

void setup() 
{
  Serial.begin(115200);

  delay(2000);

  display.begin();
  if (display.isEPD()) {
      display.setEpdMode(epd_mode_t::epd_fastest);
      display.invertDisplay(true);
      display.clear(TFT_BLACK);
  }

  display.setRotation(1);
  canvas.setTextColor(BLUE);
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);

  canvas.drawString("8SERVO HAT", 40, 20);
  canvas.drawString("Click Btn A", 40, 40);
  canvas.drawString("for release Servo", 40, 60);
  canvas.pushSprite(0, 0);

  ps_attach(on_ps3_input, on_ps3_lost);
  ps_init();

  // wifi_init();
  // now_init();

  while (!drive.begin(&Wire, 0, 26, 0x36)) 
  {
    log_i("Driver init error..");
      // canvas.drawString("Connect Error", 40, 40);
      // canvas.pushSprite(0, 0);
      // vTaskDelay(100);
      vTaskDelay(1000);
  }
  drive.enableServoPower(1);
  
  log_i("Init done.");
}

void loop() 
{
  // auto left = left_value.read();
  // auto right = right_value.read();

  //log_i("Speed:\t%d\t%d", left, right);

  delay(50);
}