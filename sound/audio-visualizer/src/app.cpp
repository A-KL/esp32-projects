#include <Lcd.h>

#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"
#include "espressif_logo.h"
#include "LovyanGFXCanvas.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });
static LovyanGFXCanvas canvas(&tft);

#include "audio.h"
#include "audio_user.h"
#include "concurrent.h"
#include "controls.h"

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
  
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);
  form.Update(canvas);

#ifdef ARDUINO
  log_w("Core %d. Free heap (KB): %f ", xPortGetCoreID(), (esp_get_free_heap_size()/1024.0));
#endif

  startTasks();
}

void loop() 
{
 // loopAudio();
  loopControls();
  form.Update(canvas);
}