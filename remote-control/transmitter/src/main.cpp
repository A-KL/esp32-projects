#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

#define CHANNELS_COUNT adc_input_pins_size + sw_two_input_pins_size + sw_three_input_pins_size

#include <SPI.h>
#include <ArduinoLog.h>

#include "printf.h"
#include "inputs.hpp"

#ifdef ESP_NOW_ENABLED
#include <esp32_now.hpp>
#endif

#ifdef RADIO_ENABLED
#include "radio.hpp"
#endif

// // Filters
// SmoothingFilter adc_inputs[ADC_CHANNELS_COUNT] =
// {
//   SmoothingFilter(adc_input_pins[0], 0),
//   SmoothingFilter(adc_input_pins[1], 0),
//   SmoothingFilter(adc_input_pins[2], 0),
//   SmoothingFilter(adc_input_pins[3], 0),
//   SmoothingFilter(adc_input_pins[4], 0),
//   SmoothingFilter(adc_input_pins[5], 0)
// };

// ThreeWaySwitchFilter sw_three_inputs[SW_THREE_CHANNELS_COUNT] =
// {
//   ThreeWaySwitchFilter(sw_three_input_pins[0], sw_three_input_pins[1]),
//   ThreeWaySwitchFilter(sw_three_input_pins[2], sw_three_input_pins[3]),
// };

// TwoWaySwitchFilter sw_two_inputs[SW_TWO_CHANNELS_COUNT] =
// {
//   TwoWaySwitchFilter(sw_two_input_pins[0]),
//   TwoWaySwitchFilter(sw_two_input_pins[1])
// };

void setup() 
{
  Serial.begin(115200);

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  printf_begin();

  inputs_init();

#ifdef RADIO_ENABLED
  radio_init();
#endif

#ifdef ESP_NOW_ENABLED
  storage_init();

  now_init();

  //now_add_peers_json();
  // const uint8_t receiveAddress[] = {0x84, 0xFC, 0xE6, 0x00, 0x27, 0x9C }; 
  // const uint8_t receiveAddress2[] = {0x94, 0xB9, 0x7E, 0xAD, 0xBD, 0xA4 };
  now_add_peer(broadcastAddress);
#endif

  delay(1000);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);

  for (auto i=0; i< CHANNELS_COUNT; i++) 
  {
    unsigned short raw = inputs[i]->read();
    Log.info(" %d\t", raw);
  #ifdef RADIO_ENABLED
    message.channels[i].value = raw;
  #endif
  #ifdef ESP_NOW_ENABLED
    data_message.channels[i].value = raw;
  #endif
  }

  Log.infoln(" ");

  radio_send_default();

  #ifdef ESP_NOW_ENABLED
  log_d("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
    data_message.channels[0].value,
    data_message.channels[1].value,
    data_message.channels[2].value,
    data_message.channels[3].value,
    data_message.channels[4].value,
    data_message.channels[5].value,
    data_message.channels[6].value,
    data_message.channels[7].value,
    data_message.channels[8].value,
    data_message.channels[9].value);

  auto res = now_send(data_message);

  if (!res) {
    //log_e("ESP_NOW: Error sending the data");
  }
  #endif
  digitalWrite(LED_BUILTIN, LOW);
}
