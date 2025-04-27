#pragma once

#include <nRF24L01.h>
#include <RF24.h>

#define RF_CHANNEL 115
#define RF_PIPE_OUT 0xE9E8F0F0E1LL

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

void radio_init()
{
  radio.begin();
  radio.setChannel(RF_CHANNEL);
  radio.setPALevel(RF24_PA_MAX);
  radio.setAutoAck(true);
 // radio.setPayloadSize(sizeof(Signal));
  radio.setDataRate(RF24_2MBPS);
  //radio.setRetries(3, 5);
  radio.openWritingPipe(RF_PIPE_OUT);
  radio.stopListening();
//#ifdef DEBUG
  radio.printDetails();
  Log.infoln("RF24 Connected: %d", radio.isChipConnected() == 1 ? "Yes" : "No");
//#endif
}

inline void radio_send_default()
{
  auto res = radio.write(&message, sizeof(Signal));
  if (!res){
    // log_d("RF24: Error sending the data"); 
 }
}