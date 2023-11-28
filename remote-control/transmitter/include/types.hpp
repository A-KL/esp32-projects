#pragma once
	
#include <map>

#ifdef __cplusplus
extern "C" {
#endif

#define CHANNELS_COUNT 16

struct channel_t {
  unsigned short value;
};

struct data_message_t {
  channel_t channels[CHANNELS_COUNT];
};

data_message_t message;

#ifdef __cplusplus
}
#endif