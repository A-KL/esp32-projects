#include <Lcd.h>

#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "RadioStation.h"
#include "espressif_logo.h"
#include "UICanvas.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });
static TFT_Canvas canvas(&tft);

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

  #if defined(I2C_SDA) and defined(I2C_SCL)
    #include "Wire.h"
    Wire.begin(I2C_SDA, I2C_SCL, 100000);
  #endif

  canvas.Init(Color::White);
  canvas.LoadFont(NotoSansBold15, sizeof(NotoSansBold15));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);
  form.Update(canvas);

#ifdef ARDUINO
  log_i("Core %d. Free heap (MB): %f ", xPortGetCoreID(), (esp_get_free_heap_size()/1024.0/1024));
#endif

  startTasks();
}

void loop() 
{
 // loopAudio();
  loopControls();
  form.Update(canvas);
}