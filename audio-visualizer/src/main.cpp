#define USE_HELIX

#include <Arduino.h>
#include "Network.h"

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "xQueueAudioStream.h"

#include "Color.h"
#include "Canvas.h"
#include "espressif-logo-featured.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
  #define TCanvas M5StackCanvas
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
  #define TCanvas TFTCanvas
#endif

#define TCanvas TFTCanvas

#include "ui.h"

TCanvas canvas;

URLStream url;
I2SStream i2s;
SPDIFStream spdif;

xQueueAudioStream<int16_t> visualization_output(audioFrameQueue);
EncodedAudioStream dec(&visualization_output, new MP3DecoderHelix());
MetaDataPrint metadata;
MultiOutput output(metadata, dec);
StreamCopy copier(output, url);

// callback for meta data
static void printMetaData(MetaDataType type, const char* str, int len){
    Serial.print("==> ");
    Serial.print(toStr(type));
    Serial.print(": ");
    Serial.println(str);

    label_track.setText(str);
}

I2SStream* initI2S(uint32_t sample_rate = 441000) {
    auto config = i2s.defaultConfig(TX_MODE);
    //config.port_no = 0;
    config.pin_ws = 25;
    config.pin_bck = 26;
    config.pin_data = 33;
    //config.pin_mck = 10;
    config.sample_rate = sample_rate;
    config.i2s_format = I2S_STD_FORMAT;
    i2s.begin(config);

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    WRITE_PERI_REG(PIN_CTRL, 0xFFF0);

    return &i2s;
}

SPDIFStream* initSPDIF(uint32_t sample_rate = 441000) {
    auto config = spdif.defaultConfig();
    config.pin_data = 27;
    config.sample_rate = sample_rate;
    config.channels = 2;
    config.bits_per_sample = 16;
    spdif.begin(config);
    return &spdif;
}

void setup(){
  Serial.begin(115200);

  canvas.Init(Color(255, 255, 255));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();

  canvas.Clear(Color(0, 0, 0));

  startAnalyzer((void*)&canvas);

  //AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  // setup I2S based on sampling rate provided by decoder
  visualization_output.begin(initI2S());

  dec.setNotifyAudioChange(visualization_output);
  dec.begin();

// mp3 radio
  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");

  metadata.setCallback(printMetaData);
  metadata.begin(url.httpRequest());
}

void loop(){
  copier.copy();
}