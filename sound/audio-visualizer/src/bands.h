#pragma once

#define BANDS_COUNT 30

struct bandData
{  
  int start;
  int witdth;
  int peak;
  int amplitude;
  int amplitude_old;

  inline int amplitude_avg()
  {
    return amplitude / witdth;
  }

  inline bool inRange(int index) const
  {
    return (start <= index) && (index < (start + witdth));
  }
};

bandData bands[BANDS_COUNT] = {
    {2, 1, 0, 0, 0},
    {3, 1, 0, 0, 0},
    {4, 1, 0, 0, 0},
    {5, 1, 0, 0, 0},
    {6, 1, 0, 0, 0},
    {7, 1, 0, 0, 0},
    {8, 1, 0, 0, 0},
    {9, 1, 0, 0, 0},
    {10, 1, 0, 0, 0},
    {11, 1, 0, 0, 0},

    {50, 1, 0, 0, 0},
    {55, 1, 0, 0, 0},
    {60, 1, 0, 0, 0},
    {65, 1, 0, 0, 0},
    {70, 1, 0, 0, 0},
    {75, 1, 0, 0, 0},
    {80, 1, 0, 0, 0},
    {85, 1, 0, 0, 0},
    {90, 1, 0, 0, 0},
    {95, 1, 0, 0, 0},

    {110, 1, 0, 0, 0},
    {125, 1, 0, 0, 0},
    {130, 1, 0, 0, 0},
    {140, 1, 0, 0, 0},
    {150, 1, 0, 0, 0},
    {160, 1, 0, 0, 0},
    {170, 1, 0, 0, 0},
    {180, 1, 0, 0, 0},
    {190, 1, 0, 0, 0},
    {200, 1, 0, 0, 0},
};