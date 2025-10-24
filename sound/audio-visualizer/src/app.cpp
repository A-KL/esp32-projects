#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include "hal_arduino.h"
#endif

#include <map>

#ifdef ILI9341_DRIVER
  #include "LGFX_ESP32_ILI9341.hpp"
#endif

#include <LGFX_TFT_eSPI.h>
  
#include "espressif_logo.h"
#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"
#include "LovyanGFXCanvas.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });

#include "hal_app.h"

#if defined ( SDL_h_ )
  static TFT_eSPI lcd (TFT_WIDTH, TFT_HEIGHT, 3);
#else
  // #include "Network.h"
  // #include "gui.h"
  // #include "audio.h"

  static TFT_eSPI lcd;
#endif

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
    #include "NotoSansBold15.h"
#endif

static LovyanGFXCanvas canvas(lcd);

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
}

void loop() 
{
  static auto _selectedAudioSource = 0;
  static auto _selectedAudioTarget = 1;

  selectAudio(_selectedAudioTarget, _selectedAudioSource);

  form.setIcon(_selectedAudioTarget, 1);
  form.setIcon(_selectedAudioSource + 2, 1);

 // while (true)
  //{
    loopAudio();
    loopControls();
  //}

  form.Update(canvas);
}