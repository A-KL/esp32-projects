#pragma once 

#include <iostream>
#include <string>

void log_e(const char* text)
{
  std::cout << text << std::endl;
}

class String
{
  public:
    String(int value) : _text(std::to_string(value))
    {}

    String(const char* value) : _text(std::string(value))
    {}

    inline operator const char* () const {
      return _text.c_str();
    }

    inline const char* c_str() const {
      return _text.c_str();
    }

  private:
    std::string _text;
};

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}