#include <math.h>
#include <Arduino.h>
#include <sbus.h>

#include <config_esp32.h>
#include <config_esp32_c3.h>

#include <driver_pwm.h>

#if CONTROL_MODE == 0
  #define INIT_MOTORS init_motors_a_b
  #define RUN_MOTOR run_motor_a_b
#elif CONTROL_MODE == 1
  #define INIT_MOTORS init_motors_a_en
  #define RUN_MOTOR run_motor_a_en
#else
  #define INIT_MOTORS init_motors_a_b_en
  #define RUN_MOTOR run_motor_a_b_en
#endif

const int MAX_DUTY_CYCLE = (int)(pow(2, MOTOR_RES) - 1);

bfs::SbusRx sbus(&Serial1, INPUT_RX_SBUS, INPUT_TX_SBUS, true);
bfs::SbusTx sbus_tx(&Serial1, INPUT_RX_SBUS, INPUT_TX_SBUS, true);
bfs::SbusData sbus_data;

bool near_zero(const int value) {
  return (abs(value) < 30);
}

/*
  Control motors via DIR and EN signals.
  Output A is Used as DIR.
*/
void init_motors_a_en()
{
  pinMode(MOTOR_0_OUTPUT_A, OUTPUT);
  pinMode(MOTOR_1_OUTPUT_A, OUTPUT);

  ledcSetup(MOTOR_0_CHANNEL_EN, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(MOTOR_1_CHANNEL_EN, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(MOTOR_0_OUTPUT_EN, MOTOR_0_CHANNEL_EN);
  ledcAttachPin(MOTOR_1_OUTPUT_EN, MOTOR_1_CHANNEL_EN);
}
/*
  Control motors via two PWM signals.
*/
void init_motors_a_b_en()
{
  pinMode(MOTOR_0_OUTPUT_A, OUTPUT);
  pinMode(MOTOR_0_OUTPUT_B, OUTPUT);

  pinMode(MOTOR_1_OUTPUT_A, OUTPUT);
  pinMode(MOTOR_1_OUTPUT_B, OUTPUT);

  ledcSetup(MOTOR_0_CHANNEL_EN, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(MOTOR_1_CHANNEL_EN, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(MOTOR_0_OUTPUT_EN, MOTOR_0_CHANNEL_EN);
  ledcAttachPin(MOTOR_1_OUTPUT_EN, MOTOR_1_CHANNEL_EN);
}
/*
  Control motors via two pwm and EN signal.
*/
void init_motors_a_b()
{
  ledcSetup(MOTOR_0_CHANNEL_A, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(MOTOR_0_CHANNEL_B, MOTOR_FREQ, MOTOR_RES);

  ledcSetup(MOTOR_1_CHANNEL_A, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(MOTOR_1_CHANNEL_B, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(MOTOR_0_OUTPUT_A, MOTOR_0_CHANNEL_A);
  ledcAttachPin(MOTOR_0_OUTPUT_B, MOTOR_0_CHANNEL_B);

  ledcAttachPin(MOTOR_1_OUTPUT_A, MOTOR_1_CHANNEL_A);
  ledcAttachPin(MOTOR_1_OUTPUT_B, MOTOR_1_CHANNEL_B);
}

void run_motor_a_b(const byte channel_a, const byte channel_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, 0);
  }
  else if (speed > 0) {
    ledcWrite(channel_a, abs(speed));
    ledcWrite(channel_b, 0);
  }
  else {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, abs(speed));
  }
}

void run_motor_a_en(const byte pin_a, const byte pin_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);

    ledcWrite(channel_en, abs(speed));
  }
}

void run_motor_a_b_en(const byte pin_a, const byte pin_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, LOW);

    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    digitalWrite(pin_b, speed > 0 ? LOW : HIGH);

    ledcWrite(channel_en, abs(speed));
  }
}

void driver_init()
{
  pinMode(INPUT_0_ADC, INPUT);
  pinMode(INPUT_1_ADC, INPUT);

  pinMode(INPUT_0_PWM, INPUT);
  pinMode(INPUT_1_PWM, INPUT);
  
  sbus.Begin();

  INIT_MOTORS();

  attachInterrupt(digitalPinToInterrupt(INPUT_0_PWM), TimerInput0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INPUT_1_PWM), TimerInput1, CHANGE);
}

void driver_loop()
{
  int output0 = 0;
  int output1 = 0;

  if (sbus.Read())
  {
    sbus_data = sbus.data();
    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    output0 = map(sbus_data.ch[MOTOR_0_SBUS_CHANNEL], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
    output1 = map(sbus_data.ch[MOTOR_1_SBUS_CHANNEL], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
  } 
  else
  { 
      int input0 = input_0_pwm.Result();
      int input1 = input_1_pwm.Result();
  
      if (input0 > 1020)
      {  
        output0 = map(constrain(input0, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      }
      else
      {
        //input0 = analogRead(INPUT_0_ADC);
        //output0 = map(input0, INPUT_ADC_MIN, INPUT_ADC_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      }

      if (input1 > 1020)
      {
        output1 = map(constrain(input1, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      }
      else
      {
        //input1 = analogRead(INPUT_1_ADC);
        //output1 = map(input1, INPUT_PWM_MIN, INPUT_PWM_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      } 
  }

//  Serial.print("output0:");
//  Serial.print(output0);
//  Serial.print(",");
//  Serial.print("output1:");
//  Serial.println(output1);

  RUN_MOTOR(MOTOR_0_OUTPUT_A, MOTOR_0_OUTPUT_B, MOTOR_0_CHANNEL_EN, output0);
  RUN_MOTOR(MOTOR_1_OUTPUT_A, MOTOR_1_OUTPUT_B, MOTOR_1_CHANNEL_EN, output1);

  delay(50);
}