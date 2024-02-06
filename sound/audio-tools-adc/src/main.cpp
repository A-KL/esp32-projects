#include <math.h>
#include <driver/i2s.h>

#include "AudioTools.h"

#include "gui.h"
#include "envelope_detector.h"

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
#define I2S_MCLK 39
 
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 44100

#define I2S_BUFFER_SIZE 128
#define I2S_SAMPLES_PER_MS I2S_SAMPLE_RATE / 1000

audio_envelope_context_t right_envelope_context;
audio_envelope_context_t left_envelope_context;

// BLCK	bck_io_num (e.g. GPIO14)
// DATA	data_in_num (e.g. GPIO32)
// LRCK	ws_io_num (e.g.GPIO15)

AudioInfo info(44100, 2, 32);
I2SStream i2sStream; // Access I2S as stream
CsvOutput<int32_t> csvStream(Serial);
StreamCopy copier(csvStream, i2sStream); // copy i2sStream to csvStream

void setup() 
{
    Serial.begin(115200);

    delay(1000);

    tft.init();
    tft.setRotation(TFT_ROTATE);
    tft.setSwapBytes(true);

    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.loadFont(NotoSansBold15);

    tft.fillScreen(TFT_BLACK);

    gui_init();

    envelope_init(right_envelope_context, I2S_SAMPLE_RATE);
    envelope_init(left_envelope_context, I2S_SAMPLE_RATE);

    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    
    auto cfg = i2sStream.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
    cfg.is_master = true;
     // this module nees a master clock if the ESP32 is master
    cfg.use_apll = false;  // try with yes
    cfg.pin_mck = 3; 
    i2sStream.begin(cfg);

    // make sure that we have the correct channels set up
    csvStream.begin(info);

    //xTaskCreate(gui_update_task, "gui_task", 2048, NULL, 1, NULL);
}

void loop() 
{
    copier.copy();
}