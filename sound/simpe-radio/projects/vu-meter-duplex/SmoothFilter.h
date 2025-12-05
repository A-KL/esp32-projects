#pragma once

#include <Smoothed.h>

template <typename T, int TSize>
class SmoothFilter 
{
  public:
    bool begin (byte mode, uint16_t smoothFactor) {
      for (auto i = 0; i < TSize; i++) {
        _values[i].begin(mode, smoothFactor);
      }
      return true;
    }

    bool add (int index, T value) {
      if (index < 0 || index > TSize) {
       // return false;
      }
      return _values[index].add(value);
    }

    inline T get (int index) {
      return _values[index].get();
    }

    inline int size() const {
      return TSize;
    }

  private:
    Smoothed<T> _values[TSize];
};