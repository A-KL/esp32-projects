#include <Lcd.h>

#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"
#include "espressif_logo.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });

#if defined ( SDL_h_ )
  #include "LovyanGFXCanvas.h"
  static TFT_eSPI lcd (TFT_WIDTH, TFT_HEIGHT, TFT_SDL_SCALE);
  static LovyanGFXCanvas canvas(&lcd);
#else
  #if defined(LOVYANGFX_HPP_) or defined(_TFT_eSPIH_)
    #include "LovyanGFXCanvas.h"
    static TFT_eSPI lcd;
    static LovyanGFXCanvas canvas(&lcd);
  #else
    #include "TFT_eSPI_Canvas.h"
    static TFT_eSPI lcd(TFT_CS, TFT_DC, TFT_RES);
    static TFT_eSPI_Canvas canvas(&lcd);
  #endif
#endif

#include "controls.h"
#include "audio.h"
#include "gui.h"

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
   #include "NotoSansBold15.h"
#endif

void setup() 
{
  log_init();

  canvas.Init(Color::White);
  canvas.LoadFont(NotoSansBold15, sizeof(NotoSansBold15));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  
  // setupWiFi();
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);

 // setupUI();

  form.Update(canvas);

#ifdef ARDUINO
  //log_e("Free heap (KB): %f", (esp_get_free_heap_size()/1024.0));
  log_w("Device ready..");
#endif
}

void loop() 
{
  static auto _selectedAudioSource = 0;
  static auto _selectedAudioTarget = 1;

  selectAudio(_selectedAudioTarget, _selectedAudioSource);

  // form.setIcon(_selectedAudioTarget, 1);
  // form.setIcon(_selectedAudioSource + 2, 1);

  //loopAudio();
  loopControls();
  //loopUI();

  form.Update(canvas);
}