#pragma once

#include "UIElement.h"
#include "UIContainer.h"
#include "UILabel.h"
#include "UISoundAnalyzer.h"

class MainForm : public UIContainer
{
public:
  MainForm() : 
    UIContainer({ 0, 0, 320, 240 }),
    _footer(UIRect({ 0, 240-23, 320, 23 }), Color({ 56, 56, 56, 0 })),
    _form(NULL)
    {}

private:
    UIContainer _footer;
    UIElement* _form;
};

class SoundMetersForm : public UIContainer
{
public:
  SoundMetersForm() : 
    UIContainer({ 0, 0, 320, 240 - 23 }), 
    _input_label({ 0, 0, 320, 25 }, "S/PDIF", NULL, 16),
    _analyzer({ 30, 25, 270, 120 })
  {
    memset(peak, 0, BANDS_COUNT);

    Add(_input_label);
    Add(_analyzer);
  }

  inline UILabel* inputLabel()
  {
    return &_input_label;
  }

  void updateBand(int band, int amplitude)
  {
    if (amplitude > AMPLITUDE_MAX) 
    {
      amplitude = AMPLITUDE_MAX;
    }

    _analyzer.Update(band, amplitude);

    if (amplitude > peak[band]) 
    {
      peak[band] = amplitude;
    }
  }

private:
  const static int BANDS_COUNT = 60;
  const static int AMPLITUDE_MAX = 255;
  
  UILabel _input_label;
  UISoundAnalyzer<BANDS_COUNT> _analyzer;
  unsigned char peak[BANDS_COUNT];;
};