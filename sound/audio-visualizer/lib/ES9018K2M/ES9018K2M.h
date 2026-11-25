#pragma once

struct es9018_handle_t;

bool ES9018K2M_init(es9018_handle_t& handle);

bool ES9018K2M_set_mute(es9018_handle_t& handle, const bool mute);

bool ES9018K2M_set_volume(es9018_handle_t& handle, const uint8_t volume);

uint8_t ES9018K2M_get_volume(es9018_handle_t& handle);

