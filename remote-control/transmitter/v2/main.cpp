#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

#define CHANNELS_COUNT adc_input_pins_size + sw_two_input_pins_size + sw_three_input_pins_size

#include <SPI.h>
#include <ArduinoLog.h>

#include "printf.h"
#include "display.hpp"

void setup() 
{
  Serial.begin(115200);

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  printf_begin();

  display_init();

  delay(500);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);

  display_update();

  digitalWrite(LED_BUILTIN, LOW);
}
