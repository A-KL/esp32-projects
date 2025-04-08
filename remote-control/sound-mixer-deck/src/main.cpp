#include <Arduino.h>

const uint8_t adc_pins[] = { A0, A1, A2, A3 };
const uint8_t adc_channels = sizeof(adc_pins) / sizeof(uint8_t);
static uint8_t adc_values[adc_channels];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
}

void loop() 
{
  for(auto i=0; i< adc_channels; i++) 
  {
    adc_values[i] = analogRead(adc_pins[i]);
  }

  // auto status = Serial.readStringUntil('\r\n');
  // auto index = 0;

  // if(status[index] == '|')
  // {
  //   String value;
  //   while (status[++index] != '|')
  //   {
  //     value+= status[index];
  //   }
  //   auto int_value = itoa();

  // }

  for(auto i=0; i< adc_channels; i++) 
  {
    Serial.print("|");
    Serial.print(adc_values[i]);
  }
  Serial.println("|");
}