//#include <Arduino.h>
//#include <map>

#include <Lcd.h>

#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"
#include "espressif_logo.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });

#include "hal_app.h"

#if defined ( SDL_h_ )
  #include "LovyanGFXCanvas.h"
  static TFT_eSPI lcd (TFT_WIDTH, TFT_HEIGHT, 3);
  static LovyanGFXCanvas canvas(&lcd);
#else
  // #include "Network.h"
  // #include "gui.h"
  // #include "audio.h"
  #ifdef LOVYANGFX_HPP_
  #include "LovyanGFXCanvas.h"
  static TFT_eSPI lcd;
  #else
  #include "TFT_eSPI_Canvas.h"
  static TFT_eSPI lcd(TFT_CS, TFT_DC, TFT_RES);
  #endif
  static LovyanGFXCanvas canvas(&lcd);
#endif

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
   #include "NotoSansBold15.h"
   #include "NotoSans_Bold7pt7b.h"
#endif

void setup() 
{
  log_init();
  //lcd.setFont(&NotoSans_Bold7pt7b);
  //lcd.setTextDatum(CC_DATUM);
  canvas.Init(Color::White);
  canvas.LoadFont(NotoSansBold15, sizeof(NotoSansBold15));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  
  setupWiFi();
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);
  canvas.DrawFilledRect(20, 20, 50, 20, Color::Blue);

  form.Update(canvas);

  #ifdef ARDUINO
  log_e("Free heap (KB): %f", (esp_get_free_heap_size()/1024.0));
  #endif
}

void loop() 
{
  static auto _selectedAudioSource = 0;
  static auto _selectedAudioTarget = 1;

  selectAudio(_selectedAudioTarget, _selectedAudioSource);

  form.setIcon(_selectedAudioTarget, 1);
  form.setIcon(_selectedAudioSource + 2, 1);

  loopAudio();
  loopControls();

  form.Update(canvas);
}