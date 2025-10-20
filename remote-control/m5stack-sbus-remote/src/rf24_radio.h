#pragma once

#include "printf.h"
#include "RF24.h"

#define CHANNELS_COUNT 10
#define PIPE_IN 0xE9E8F0F0E1LL
#define CHANNEL_VALUE_MAX 4095

//const uint64_t pipeIn = 0xE9E8F0F0E1LL;

// Protocol
struct Channel {
  unsigned short value:12;
};
struct Signal {
  Channel channels[CHANNELS_COUNT];
};
Signal received;

static RF24 radio(5, 2);
static uint64_t lastRecvTime = 0;

void setupRadio() {
  radio.begin();
  radio.setChannel(115);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(1, PIPE_IN);
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

bool RF24_IsReceived() {
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