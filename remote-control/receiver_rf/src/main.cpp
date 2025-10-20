#include <SPI.h>
#include <Wire.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CHANNEL_VALUE_MAX 4095

#ifdef SBUS_ENABLE
  #include <sbus.h>
#endif

#ifdef PPM_ENABLE
  #include <PPMEncoder.h>
#endif

#ifdef ESP32
  #include <ESP32Servo.h>

  #define LED_BUILTIN 5
  #define RF_CE_PIN 2
  #define RF_CSN_PIN 4
  #define SBUS_PINS 03,01
  #define SBUS_SERIAL Serial1
  #define PPM_PIN A3

  int servo_pins[CHANNELS_COUNT] = {10, 9, 8, 7, 6, 5, 4, 3, 2, A0};
#endif

#ifdef ARDUINO_AVR_NANO
  #ifdef SERVO_ENABLE
    #include <Servo.h>
  #endif

  #define RF_CE_PIN 10
  #define RF_CSN_PIN 9
  #define SBUS_PINS
  #define SBUS_SERIAL Serial
  #define PPM_PIN A3

  int servo_pins[CHANNELS_COUNT] = { 3, 5, 6, 4, 2, 7, 8, A0, A1, A2 };

#elif ARDUINO_AVR_NANO
  #ifdef SERVO_ENABLE
    #include <Servo.h>
  #endif

  #define RF_CE_PIN 4
  #define RF_CSN_PIN 2
  #define SBUS_PINS
  #define SBUS_SERIAL Serial
  #define PPM_PIN A3

  int servo_pins[CHANNELS_COUNT] = { 3, 5, 6, 9, 10, 7, 8, A0, A1, A2 };
#endif

#ifdef SERVO_ENABLE
  Servo servos[CHANNELS_COUNT];
#endif

// SBUS
#ifdef SBUS_ENABLE


  bfs::SbusTx sbus_tx(&SBUS_SERIAL, SBUS_PINS, 1);
  bfs::SbusData sbus_data;
#endif

#if SERVO_DRIVER_ENABLE
  Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);
#endif

// Radio
const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(RF_CE_PIN, RF_CSN_PIN);

// Protocol
struct Channel {
  unsigned short value:12;
};
struct Signal {
  Channel channels[CHANNELS_COUNT];
};
Signal received;

// Main
unsigned long lastRecvTime = 0;

void setupServo() {
  #ifdef SERVO_ENABLE
    for (auto i=0; i< CHANNELS_COUNT; i++) {
      servos[i].attach(servo_pins[i]);
    }
  #endif
}

void setupServoDriver() {
  #ifdef SERVO_DRIVER_ENABLE
    pca.begin();
    pca.setOscillatorFrequency(27000000);
    pca.setPWMFreq(50);
    delay(10);
  #endif
}

void resetReceived() {
  for (auto i=0; i< CHANNELS_COUNT; i++) {
    received.channels[i].value = 0;
  }
}

void setupRadio() {
  radio.begin();
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();

  #ifdef DEBUG
    printf_begin();
    radio.printDetails();
    Serial.print("RF24 Connected: ");
    Serial.println(radio.isChipConnected() == 1 ? "Yes" : "No");
  #endif
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  #ifdef SBUS_ENABLE
    sbus_tx.Begin();
  #endif

  #ifdef PPM_ENABLE
    ppmEncoder.begin(PPM_PIN);
  #endif

  setupServo();
  setupServoDriver();

  resetReceived();
  setupRadio();
}

void receiveRadio() {
  while (radio.available()) {
    radio.read(&received, sizeof(Signal));
    lastRecvTime = millis();
  }
}

void loop() {
  receiveRadio();
  
  if (millis() - lastRecvTime > 50 ) {
    resetReceived();
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  #ifdef DEBUG
    for (auto i=0; i< CHANNELS_COUNT; i++) {
      Serial.print("CH");
      Serial.print(i);
      Serial.print(':');
      Serial.print(received.channels[i].value);
      Serial.print(',');
    }
    Serial.println();
  #endif

  #ifdef SBUS_ENABLE
    for (auto i=0; i< CHANNELS_COUNT; i++) {
      auto servo_value = map(received.channels[i].value, 0, CHANNEL_VALUE_MAX, 1000, 2000);
      sbus_data.ch[i] = servo_value;
    }
    sbus_tx.data(sbus_data);
    sbus_tx.Write();
  #endif

  #ifdef SERVO_DRIVER_ENABLE
    for (auto i=0; i< CHANNELS_COUNT; i++) {
      auto servo_value = map(received.channels[i].value, 0, CHANNEL_VALUE_MAX, 150, 600);
      pca.setPWM(i, 0, servo_value);
      //pwm.writeMicroseconds(servonum, microsec); 500, 2500
    }
  #endif

  #ifdef SERVO_ENABLE
    for (auto i=0; i< CHANNELS_COUNT; i++) {
      auto servo_value = map(received.channels[i].value, 0, CHANNEL_VALUE_MAX, 500, 2500);
      servos[i].writeMicroseconds(servo_value);
    }
  #endif
}
