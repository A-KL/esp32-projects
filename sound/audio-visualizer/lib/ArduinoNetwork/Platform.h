#pragma once

#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#define ARDUINO_EMULATOR_WINDOWS 0
#define ARDUINO_EMULATOR_LINUX 0
#elif defined(_WIN32)
#define ARDUINO_EMULATOR_WINDOWS 1
#define ARDUINO_EMULATOR_LINUX 0
#elif defined(__linux__) || defined(__APPLE__)
#define ARDUINO_EMULATOR_WINDOWS 0
#define ARDUINO_EMULATOR_LINUX 1
#else
#define ARDUINO_EMULATOR_WINDOWS 0
#define ARDUINO_EMULATOR_LINUX 0
#endif
