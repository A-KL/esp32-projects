#include <SPI.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>

#include <config_esp32.hpp>
#include <filters.hpp>
#include <esp32_now.hpp>

#define RF_CHANNEL 115
#define RF_PIPE_OUT 0xE9E8F0F0E1LL

#define ADC_CHANNELS_COUNT 6
#define SW_TWO_CHANNELS_COUNT 2
#define SW_THREE_CHANNELS_COUNT 2

#define CHANNELS_COUNT ADC_CHANNELS_COUNT + SW_TWO_CHANNELS_COUNT + SW_THREE_CHANNELS_COUNT

#ifdef ESP32
  int adc_input_pins[] = { 
      36, 39, // Left, Right Knobs
      34, 35, // Right Joystick - Y, X
      32, 33  // Left Joystick - Y, X  
  };

  int sw_three_input_pins[SW_THREE_CHANNELS_COUNT * 2] = { 
      2, 0, // Left Switch
      25, 26  // Right Switch
  };

  int sw_two_input_pins[SW_TWO_CHANNELS_COUNT] = { 
      15, // Left Joystick - Button
      13, // Right Joystick - Button
  };
#endif

// Radio
RF24 radio(RF_CE_PIN, RF_CSN_PIN);

// Protocol
struct Channel {
  unsigned short value:12;
};
struct Signal {
  Channel channels[CHANNELS_COUNT];
};

Signal message;

// Filters
SmoothingFilter adc_inputs[ADC_CHANNELS_COUNT] =
{
  SmoothingFilter(adc_input_pins[0], 0),
  SmoothingFilter(adc_input_pins[1], 0),
  SmoothingFilter(adc_input_pins[2], 0),
  SmoothingFilter(adc_input_pins[3], 0),
  SmoothingFilter(adc_input_pins[4], 0),
  SmoothingFilter(adc_input_pins[5], 0)
};

ThreeWaySwitchFilter sw_three_inputs[SW_THREE_CHANNELS_COUNT] =
{
  ThreeWaySwitchFilter(sw_three_input_pins[0], sw_three_input_pins[1]),
  ThreeWaySwitchFilter(sw_three_input_pins[2], sw_three_input_pins[3]),
};

TwoWaySwitchFilter sw_two_inputs[SW_TWO_CHANNELS_COUNT] =
{
  TwoWaySwitchFilter(sw_two_input_pins[0]),
  TwoWaySwitchFilter(sw_two_input_pins[1])
};

InputFilter* inputs[CHANNELS_COUNT];

void setupInputs() {
  auto index = 0;

  for(auto i=0; i<ADC_CHANNELS_COUNT; ++i) {
    inputs[index++] = &adc_inputs[i];
  }

  for(auto i=0; i<SW_THREE_CHANNELS_COUNT; ++i) {
    inputs[index++] = &sw_three_inputs[i];
  }

  for(auto i=0; i<SW_TWO_CHANNELS_COUNT; ++i) {
    inputs[index++] = &sw_two_inputs[i];
  }
}

void setupRadio() {
  radio.begin();
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(true);
 // radio.setPayloadSize(sizeof(Signal));
  radio.setDataRate(RF24_2MBPS);
  //radio.setRetries(3, 5);
  radio.openWritingPipe(RF_PIPE_OUT);
  radio.stopListening();
  printf_begin();
  radio.printDetails();
  Serial.print("RF24 Connected: ");
  Serial.println(radio.isChipConnected() == 1 ? "Yes" : "No");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  setupInputs();

  setupRadio();
  setupEspNow();

  delay(3000);
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);

  for (auto i=0; i< CHANNELS_COUNT; i++) {
    auto raw = inputs[i]->read();

    message.channels[i].value = raw;
    data_message.channels[i].value = raw;
  }

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

  auto res = sendEspNow(data_message);

  if (!res) {
    //log_e("ESP_NOW: Error sending the data");
  }

  digitalWrite(LED_BUILTIN, LOW);

  delay(10);

  return;

  res = radio.write(&message, sizeof(Signal));

  if (!res){
      log_d("RF24: Error sending the data"); 
  }
}
