#include <Arduino.h>

#include "lcd_ssd1306.h"
#include "parsing.h"

const uint8_t adc_pins[] = { A0, A1, A2, A3 };
const uint8_t adc_channels = sizeof(adc_pins) / sizeof(uint8_t);
static uint16_t adc_values[adc_channels];

long display_last_update = millis();
long long last_received = millis();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display_text("STRM-DECK");
  delay(1000);

  display.clearDisplay();
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() 
{
  for(auto i=0; i< adc_channels; i++) {
    adc_values[i] = analogRead(adc_pins[i]);
  }

  // if ((millis() - display_last_update) > 150) {
  //   display_last_update = millis();
  //   display_percentage("XBOX", map(adc_values[0], 0, 1020, 0, 100));
  // }

  if (Serial.available()) {
    auto command = Serial.readStringUntil('\r');
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 2);
    display.clearDisplay();

    String id;
    String property;
    String value;

    command = parse_input(command, id, property, value);
    display.print(id);
    display.print(": ");
    display.print(value);
    display.print(" | ");

    command = parse_input(command, id, property, value);
    display.println(value);

    command = parse_input(command, id, property, value);
    display.print(id);
    display.print(": ");
    display.print(value);
    display.print(" | ");

    command = parse_input(command, id, property, value);
    display.println(value);

    display.display();

    last_received = millis();
  }

  if ((millis() - last_received) > 1000) {
    display_text("NO SIGNAL");
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

  // for (auto i=0; i< (adc_channels-1); i++) 
  // {
  //   Serial.print(adc_values[i]);
  //   Serial.print("|");
  // }
  // Serial.print(adc_values[(adc_channels-1)]);
  // Serial.println();
}