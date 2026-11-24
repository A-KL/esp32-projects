#include <Lcd.h>
#include <Color.h>
#include <LcdCanvas.h>

#include "MainForm.h"
#include "espressif_logo.h"
#include "TrueType/NotoSans_Bold.h"

static MainForm form({ 0, 0, TFT_WIDTH, TFT_HEIGHT });
static LcdCanvas<Color> canvas(tft);

#include "audio.h"
#include "codec_volume.h"

// #include "concurrent.h"
#include "controls.h"

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
  #if ARDUINO_USB_CDC_ON_BOOT
  // A zero timeout keeps optional logging from delaying application startup
  // when USB CDC is not connected or the host is not draining output.
  Serial.setTxTimeoutMs(0);
  #endif
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void setup() 
{
  log_init();

  #if defined(I2C_SDA) and defined(I2C_SCL)
    #include "Wire.h"
    Wire.begin(I2C_SDA, I2C_SCL, 100000);
  #endif

  canvas.Init(Color::White);
  canvas.LoadFont(NotoSans_Bold, sizeof(NotoSans_Bold));

  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);
  canvas.DrawText(0, 220, 320, 20, "Connecting...", Color::Black, Color::White);
  
  setupControls();
  setupCodec();
  setupAudio();

  canvas.Clear(Color::Black);
  form.Update(canvas);

# ifdef ARDUINO_ARCH_ESP32
  LOGI("Core %d. Free heap (MB): %f ", xPortGetCoreID(), (esp_get_free_heap_size()/1024.0/1024));
# endif

  // startTasks();
}

void loop() 
{
  loopAudio();
  loopControls();
  form.Update(canvas);
}