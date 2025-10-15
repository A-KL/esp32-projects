#pragma once

enum epd_mode_t : int
{
  epd_quality = 1,
  epd_text    = 2,
  epd_fast    = 3,
  epd_fastest = 4,
};



class TFT_eSPI_GFX : public TFT_eSPI
{
  public:
    TFT_eSPI_GFX()
    {
    #ifdef PWR_EN_PIN
      pinMode(PWR_EN_PIN, OUTPUT);
      digitalWrite(PWR_EN_PIN, HIGH);
    #endif

    #ifdef PWR_ON_PIN
      pinMode(PWR_ON_PIN, OUTPUT);
      digitalWrite(PWR_ON_PIN, HIGH);
    #endif

    #ifdef TFT_BLK_PIN
      pinMode(TFT_BLK_PIN, OUTPUT);
      digitalWrite(TFT_BLK_PIN, HIGH);
    #endif
    }

    inline void display()
    { }

    inline bool isEPD() {
      return false;
    }

    inline void setEpdMode(epd_mode_t epd_mode)
    {}

    
};
