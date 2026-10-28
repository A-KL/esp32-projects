#include <Lcd.h>
#include <Color.h>
#include <Canvas.h>
#include <LcdCanvas.h>

#include "MainForm.h"
#include "RadioStation.h"
#include "espressif_logo.h"
#include "TrueType/NotoSans_Bold.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });
static LcdCanvas<Color> canvas(tft);

#include "audio.h"
#include "audio_volume.h"
#include "concurrent.h"
#include "controls.h"

void setup() 
{
  log_init();

  #if defined(I2C_SDA) and defined(I2C_SCL)
    #include "Wire.h"
    Wire.begin(I2C_SDA, I2C_SCL, 100000);
  #endif

  canvas.Init(Color::White);
  // canvas.LoadFont(NotoSansBold15, sizeof(NotoSansBold15));
  canvas.LoadFont(NotoSans_Bold, sizeof(NotoSans_Bold));

  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  canvas.DrawText(0, 220, 320, 20, "Connecting...", Color::Black, Color::White);
  
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);
  form.Update(canvas);

// #ifdef ARDUINO
#ifndef IS_DESKTOP 
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