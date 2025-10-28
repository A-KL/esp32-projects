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
  #include "TFT_eSPI_Canvas.h"
  // #include "Network.h"
  // #include "gui.h"
  // #include "audio.h"

  static TFT_eSPI lcd(TFT_CS, TFT_DC, TFT_RES);
  static TFT_eSPI_Canvas canvas(&lcd);
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
  canvas.DrawFilledRect(20, 20, 50, 20, Color::Blue);

  form.Update(canvas);
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