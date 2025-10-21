#ifdef ARDUINO
    #include <Arduino.h>
    #ifdef ESP_PLATFORM
//        #include "hal_esp32.h"
    #endif
#else
    #include "hal_arduino.h"
#endif

#include "espressif_logo.h"

#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"

#include <LGFX_TFT_eSPI.h>
#include <LGFX_AUTODETECT.hpp>

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });

#if defined ( SDL_h_ )
  #include "LovyanGFXCanvas.h"
  #include "hal_app.h"

  static TFT_eSPI lcd ( TFT_WIDTH, TFT_HEIGHT, 2 );
  static LovyanGFXCanvas canvas(lcd);
#else
  #include "TFTCanvas.h"
  #include "Network.h"

  //#include "AdcAudioDevice.h"
  #include "gui.h"
  #include "audio.h"

  static TFT_eSPI lcd;
  static TFTCanvas canvas(&lcd);

  //AdcAudioDevice adc((float*)vReal_l, (float*)vImag_l, SAMPLES, samplig_rate);
#endif

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
    #include "NotoSansBold15.h"
#endif

void setup() 
{
  log_init();

  canvas.Init(Color::White);
  canvas.LoadFont(NotoSansBold15);
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  
  setupWiFi();
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);

  form.Update(canvas);

 // startUI((void*)&canvas);
}

void loop() 
{
  static auto _selectedAudioSource = 0;
  static auto _selectedAudioTarget = 1;

  selectAudio(_selectedAudioTarget, _selectedAudioSource);

  form.setIcon(_selectedAudioTarget, 1);
  form.setIcon(_selectedAudioSource + 2, 1);

 // while (true)
  {
    loopAudio();
    loopControls();
  }

  form.Update(canvas);
}