#include <SPI.h>
#include <WiFi.h>
#include "printf.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <esp_now.h>

#define CHANNELS_COUNT 6
#define RF_CHANNEL 115

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
      32, 33, //  2, 4,   // Left Joystick - Y, X
     // 14, 12, // Right Joystick - Y, X
      34, 35, // Left, Right Switches
    //  15,     // Left Joystick - Button
    //  13,     // Right Joystick - Button
  };

  #define CHANNELS_COUNT_T 16

  struct channel_t {
    unsigned short value;
  };

  struct data_message_t {
    channel_t channels[CHANNELS_COUNT_T];
  };

  data_message_t data_message;
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
  // SmoothingFilter(input_pins[6], 0),
  // SmoothingFilter(input_pins[7], 0),
  // SmoothingFilter(input_pins[8], 0),
  // SmoothingFilter(input_pins[9], 0)
};

const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t receiveAddress[] = {0x84, 0xFC, 0xE6, 0x00, 0x27, 0x9C };

esp_now_peer_info_t peerInfo;

void EspNowOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  log_d("Last Packet Send Status:%s", status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

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

void setupEspNow() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  log_i("ESP-NOW initialized");

  esp_now_register_send_cb(EspNowOnDataSent);

  memcpy(peerInfo.peer_addr, receiveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    log_e("Failed to add peer");
    return;
  }
}

void sendEspNow(const data_message_t& data)
{
  esp_err_t result = esp_now_send(receiveAddress, (uint8_t *) &data, sizeof(data_message_t));
   
  if (result != ESP_OK) {
    log_e("Error sending the data");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  setupRadio();
  setupEspNow();

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
    message.channels[i].value = raw;
    data_message.channels[i].value = raw;
  }

  log_d("%d\t%d\t%d\t%d\t%d\t%d\t%d",
    data_message.channels[0].value,
    data_message.channels[1].value,
    data_message.channels[2].value,
    data_message.channels[3].value,
    data_message.channels[4].value,
    data_message.channels[5].value);

  sendEspNow(data_message);

  return;

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
