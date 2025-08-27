#include <Arduino.h>

#ifndef DEVICE_ID
#error "Device-ID is not defined!"
#endif

#ifndef CONTROLS_NAME
#define CONTROLS_NAME config_json
#endif
#define CONTROLS_FILE <CONTROLS_NAME.h>
#include CONTROLS_FILE

#define Q(x) #x
#define QUOTE(x) Q(x)

#define HOSTNAME_PREFIX esp32
#define HOSTNAME QUOTE(HOSTNAME_PREFIX-DEVICE_ID)

#include <arduino-timer.h>

#include <lcd.h>
#include <led.h>
#include <network.h>
//#include <web_server.h>
#include <diagnostics.h>

#include <pwm_input.h>
#include <sbus_input.h>
#include <enow_input.h>
#include <xbox_input.h>

#include <motor_output.h>
#include <servo_output.h>
#include <lego_servo_output.h>

diagnostic_scope_t scope;

static auto input_watchdog = timer_create_default();

#ifdef LED_PIN
static auto led = led_animation_t(LED_PIN, input_watchdog);
#endif

#ifdef RGB_LED_PIN
static auto rgb_led = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_RGB + NEO_KHZ800);
static auto led = rgb_animation_t(rgb_led, input_watchdog);
#endif

bool halt(void*)
{
  // Motors
  motors_stop();

  // Lego
  lego_servos_stop();

  // Servos
  servos_attach(false);

  // Led: no connection
  led.input_lost();

//  log_i("Halt!");

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(HOSTNAME);
  sleep(2);

  scope.begin();

  config_init();
  init_wifi();  
  lcd_init();

  pwm_in_init();
  sbus_init();
  enow_init();

  xbox_init();

  motors_init();
  servos_init();
  lego_servos_init();

  scope.finish("Initialization - DONE");
}

void loop() 
{
  static int16_t inputs[32];

  static int16_t outputs_motors[motors_count];
  static int16_t outputs_servo[servos_count];
  static int16_t outputs_lego_servo[lego_servos_count];

 // scope.begin();
  static auto halt_task = (void*)NULL;
  auto input_received = true;

  //  SBUS
  if (sbus_receive(inputs) > 0) 
  {
    // Motors
    controls_map_inputs(sbus, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(sbus, inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, servos_count);

    // Lego Servo
    // controls_map_inputs(sbus, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    //lego_servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else if (enow_receive(inputs) > 0)
  {
    // Motors
    controls_map_inputs(esp_now, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
    controls_map_inputs(esp_now, inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_servo, servos_count);

    // Lego Servo
    // controls_map_inputs(esp_now, inputs, servo_lego, outputs_lego_servo, servos_count);
    // lego_servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_lego_servo, lego_servos_count);
  } 
  else if (pwm_receive(inputs))
  {
    // Motors
    controls_map_inputs(pwm, inputs, dc, outputs_motors, motors_count);
    write_motors<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_motors, motors_count);

    // Lego Servo
    // controls_map_inputs(pwm, inputs, servo_lego, outputs_lego_servo, lego_servos_count);
    // lego_servos_write<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_lego_servo, lego_servos_count);
    delay(15);
  }
  else if (xbox_receive(inputs))
  {

  }
  else
  {
    if (!halt_task) {
      halt_task = input_watchdog.every(1000, halt);
    }
    input_received = false;
  }

  if (input_received && halt_task)
  {
    input_watchdog.cancel(halt_task);
    halt_task = NULL;
    led.input_received();
  }

  lcd_display(inputs, 32);

  input_watchdog.tick();

  //scope.finish("Loop");
}