#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t adc_pins[] = { A0, A1, A2, A3 };
const uint8_t adc_channels = sizeof(adc_pins) / sizeof(uint8_t);
static uint16_t adc_values[adc_channels];


long display_last_update = millis();

void display_percentage(const char* label, uint8_t perc)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds("%", 0, 0, &x1, &y1, &w, &h);

  display.clearDisplay();
  display.setTextSize(2);
  
  display.fillRect(0,0, SCREEN_WIDTH/2 - 5, SCREEN_HEIGHT, WHITE);
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

  display.setTextColor(BLACK);
  display.setCursor(5, 8);
  display.print(label);

  display.setTextColor(WHITE);

  auto chars_count = perc < 10 ? 1 : perc < 100 ? 2 : 3;

  display.setCursor(SCREEN_WIDTH - (w * (chars_count + 1)) - 10, 8);
  display.print(perc);
  display.println("%");

  display.display();
}

void display_text(const char* label)
{
  display.clearDisplay();
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);     // text color
  display.setCursor(5, 8);        // position to display
  display.println(label);
  display.display();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display_text("SND MIXER");
  delay(2000);
}

void loop() 
{
  for(auto i=0; i< adc_channels; i++) {
    adc_values[i] = analogRead(adc_pins[i]);
  }

  if ((millis() - display_last_update) > 150) {
    display_last_update = millis();
    display_percentage("XBOX", map(adc_values[0], 0, 1020, 0, 100));
  }

  if (Serial.available()) {
    auto command = Serial.readStringUntil('\r');
    
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

  for (auto i=0; i< (adc_channels-1); i++) 
  {
    Serial.print(adc_values[i]);
    Serial.print("|");
  }
  Serial.print(adc_values[(adc_channels-1)]);
  Serial.println();
}