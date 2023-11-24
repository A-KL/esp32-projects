#include <SPI.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>

#define CHANNELS_COUNT 10
#define RF_CHANNEL 115
#define DEBUG 1

#ifdef ARDUINO_AVR_NANO
  #define RF_CE_PIN 7
  #define RF_CSN_PIN 8
  #define ADC_MAX 1024
#endif

#ifdef ESP32
  #define LED_BUILTIN 5
  #define RF_CE_PIN 16
  #define RF_CSN_PIN 17
  #define ADC_MAX 4095

  int input_pins[CHANNELS_COUNT] = { 
      36, 39, // Left, Right Knobs
      2, 4,   // Left Joystick - Y, X
      14, 12, // Right Joystick - Y, X
      34, 35, // Left, Right Switches
      15,     // Left Joystick - Button
      13,     // Right Joystick - Button
  };
#endif

// Radio
const uint64_t pipeOut = 0xE9E8F0F0E1LL;
RF24 radio(RF_CE_PIN, RF_CSN_PIN); // select CE,CSN pin

// Protocol
struct Channel {
  unsigned short value:12;
};
struct Signal {
  Channel channels[CHANNELS_COUNT];
};

Signal message;

// Filters
class SmoothingFilter
{
  public:
    SmoothingFilter(const byte channel, const int padding = 0, const int error = 4)
    {
      _channel = channel;
      _padding = padding;
      _error = error;
      
      for(auto i = 0; i < _size; i++)
      {
        _readings[i] = 0;
      }
    }
    
    int read()
    {
      _total -= _readings[_index];
      
      auto read = analogRead(_channel) + _padding;
      _readings[_index] = abs(_readings[_index] - read) > _error ? read : _readings[_index];
      
      _total += _readings[_index];
      
      _index++;
      
      if (_index >= _size) {
        _index = 0;
      }
      
      return _total / _size;
    }
    
  private:
    const static int _size = 10;
    int _error = 3;
    int _padding = 0;
    byte _channel = 0;
    int _total = 0;
    int _index = 0;
    int _readings[_size];
};

SmoothingFilter inputs[CHANNELS_COUNT] 
{ 
  SmoothingFilter(input_pins[0], 0),
  SmoothingFilter(input_pins[1], 0),
  SmoothingFilter(input_pins[2], 0),
  SmoothingFilter(input_pins[3], 0),
  SmoothingFilter(input_pins[4], 0),

  SmoothingFilter(input_pins[5], 0),
  SmoothingFilter(input_pins[6], 0),
  SmoothingFilter(input_pins[7], 0),
  SmoothingFilter(input_pins[8], 0),
  SmoothingFilter(input_pins[9], 0)
};

void setupRadio() {
  radio.begin();
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(true);
 // radio.setPayloadSize(sizeof(Signal));
  radio.setDataRate(RF24_2MBPS);
  //radio.setRetries(3, 5);
  radio.openWritingPipe(pipeOut);
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

  setupRadio();

  delay(5000);
}

int mapAnalogValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);

  if (val < middle)
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);

  return (reverse ? 255 - val : val);
}

void loop() {
  for (auto i=0; i< CHANNELS_COUNT; i++) {
    auto raw = inputs[i].read();
    Serial.print("CH");
    Serial.print(i);
    Serial.print(':');
    Serial.print(raw);
    Serial.print(',');
    message.channels[i].value = raw;
  }
  Serial.println();

  // for (auto i=0; i< CHANNELS_COUNT; i++) {
  //   Serial.print("CH");
  //   Serial.print(i);
  //   Serial.print(':');
  //   Serial.print(message.channels[i]);
  //   Serial.print(',');
  // }
  // Serial.println();

  auto res = radio.write(&message, sizeof(Signal));

  if (!res){
      Serial.println("Tx failed");
      digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
      digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(10);
}
