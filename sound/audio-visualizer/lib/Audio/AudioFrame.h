#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {
    int16_t left;
    int16_t right;
} AudioFrame;