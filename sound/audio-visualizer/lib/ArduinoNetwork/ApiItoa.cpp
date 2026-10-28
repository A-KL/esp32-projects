#include "Platform.h"

#if ARDUINO_EMULATOR_WINDOWS || ARDUINO_EMULATOR_LINUX
#include "api/itoa.h"

#include <charconv>
#include <climits>
#include <cstring>
#include <system_error>

namespace {

template <typename Integer>
char* convertInteger(Integer value, char* buffer, int radix) {
  if (buffer == nullptr || radix < 2 || radix > 36) {
    return nullptr;
  }

  char converted[sizeof(Integer) * CHAR_BIT + 2];
  const auto result =
      std::to_chars(converted, converted + sizeof(converted), value, radix);
  if (result.ec != std::errc()) {
    return nullptr;
  }

  const auto length = static_cast<size_t>(result.ptr - converted);
  std::memcpy(buffer, converted, length);
  buffer[length] = '\0';
  return buffer;
}

}  // namespace

extern "C" {

char* itoa(int value, char* buffer, int radix) {
  return convertInteger(value, buffer, radix);
}

char* ltoa(long value, char* buffer, int radix) {
  return convertInteger(value, buffer, radix);
}

char* utoa(unsigned value, char* buffer, int radix) {
  return convertInteger(value, buffer, radix);
}

char* ultoa(unsigned long value, char* buffer, int radix) {
  return convertInteger(value, buffer, radix);
}

}
#endif
