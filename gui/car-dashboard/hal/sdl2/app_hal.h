#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void hal_setup(void);
void hal_loop(void);

float hal_get_altitude(void);
float hal_get_pitch(void);

void delay(unsigned long);

#ifdef __cplusplus
} /* extern "C" */
#endif
