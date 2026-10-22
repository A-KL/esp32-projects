#pragma once 

#include <iostream>
#include <string>

#define _min(a,b) ((a)<(b)?(a):(b))  
#define _max(a,b) ((a)>(b)?(a):(b))
#define _abs(x) ((x)>0?(x):-(x))  // abs() comes from STL
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  // round() comes from STL
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

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

// float map(float x, float in_min, float in_max, float out_min, float out_max) {
//     const float run = in_max - in_min;
//     if(run == 0){
//         log_e("map(): Invalid input range, min == max");
//         return -1; // AVR returns -1, SAM returns 0
//     }
//     const float rise = out_max - out_min;
//     const float delta = x - in_min;
//     return (delta * rise) / run + out_min;
// }

// long map(long x, long in_min, long in_max, long out_min, long out_max) {
//   const long run = in_max - in_min;
//   if(run == 0){
//       log_e("map(): Invalid input range, min == max");
//       return -1; // AVR returns -1, SAM returns 0
//   }
//   const long rise = out_max - out_min;
//   const long delta = x - in_min;
//   return (delta * rise) / run + out_min;
// }