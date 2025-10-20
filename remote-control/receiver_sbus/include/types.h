#pragma once
	
#include <map>

#define CHANNELS_COUNT 16

struct channel_t {
  unsigned short value;
};

struct data_message_t {
  channel_t channels[CHANNELS_COUNT];
};

data_message_t message;