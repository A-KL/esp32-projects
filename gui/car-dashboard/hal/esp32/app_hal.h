#pragma once

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*update_callback) (void *data);

void hal_setup(void);
void hal_loop(void);

void updates(update_callback fn);

#ifdef __cplusplus
} /* extern "C" */
#endif
