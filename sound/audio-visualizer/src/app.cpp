// #include <limits.h>
// #include <sstream>

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
#include "AudioFrame.h"
#include "RadioStation.h"

#include <LGFX_TFT_eSPI.h>
#include <LGFX_AUTODETECT.hpp>

#if defined ( SDL_h_ )
  #include "SDLCanvas.h"

  static TFT_eSPI lcd ( 320, 240, 2 );
  static SDLCanvas canvas(&lcd);
#else
  #include "TFTCanvas.h"
  #include "Network.h"

  //#include "AdcAudioDevice.h"
  #include "ui.h"
  #include "audio.h"

  static TFT_eSPI lcd;
  static TFTCanvas canvas(&lcd);
#endif

//AdcAudioDevice adc((float*)vReal_l, (float*)vImag_l, SAMPLES, samplig_rate);

void setup() 
{
  static auto _selectedAudioSource = 0;
  static auto _selectedAudioTarget = 1;

  Serial.begin(115200);

  canvas.Init(Color::White);
  canvas.SetFont(0, 1);
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  
  setupWiFi();
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);
  startUI((void*)&canvas);

  while (true)
  {
    selectAudio(_selectedAudioTarget, _selectedAudioSource);

    form.setIcon(_selectedAudioTarget, 1);
    form.setIcon(_selectedAudioSource + 2, 1);

    while (true)
    {
      loopAudio();
      loopControls();
    }
  }
}

void loop() {
}