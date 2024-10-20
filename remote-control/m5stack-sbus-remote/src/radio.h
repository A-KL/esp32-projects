#ifndef radio_h
#define radio_h

#include "printf.h"
#include "RF24.h"

#define CHANNELS_COUNT 10

const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(5, 2);

unsigned long lastRecvTime = 0;

// Protocol
struct Channel {
  unsigned short value:12;
};
struct Signal {
  Channel channels[CHANNELS_COUNT];
};
Signal received;

#define CHANNEL_VALUE_MAX 4095

void setupRadio() {
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();
  printf_begin();
  radio.printDetails();
  Serial.print("RF24 Connected: ");
  Serial.println(radio.isChipConnected() == 1 ? "Yes" : "No");
}

void resetReceived() {
  for (auto i=0; i< CHANNELS_COUNT; i++) {
    received.channels[i].value = 0;
  }
}

void receiveRadio() {
  while (radio.available()) {
    radio.read(&received, sizeof(Signal));
    lastRecvTime = millis();
  }
}

bool isReceived() {
    receiveRadio();

    auto not_ok = (millis() - lastRecvTime) > 100; 
    if (not_ok) {
        resetReceived();
       // digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
       // digitalWrite(LED_BUILTIN, LOW);
    }

    return !not_ok;
}

#endif