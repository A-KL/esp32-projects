#pragma once

void hal_setup(void);
void hal_loop(void);

void hal_log_cb(const char*);

float hal_get_altitude(void);
float hal_get_pitch(void);