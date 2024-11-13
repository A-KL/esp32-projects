// #define NO_TRACE
// #define LOG_NO_MSG
// #define COPY_LOG_OFF
// #define LOG_LEVEL AudioLogger::Error

#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include "VuOutput.h"

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
I2SStream i2s;
VuMeter<int32_t> vu;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream dec(&nfc, new MP3DecoderHelix());
StreamCopy copier(dec, url);

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);

  //tft.setFreeFont(&Orbitron_Medium_20);
  tft.loadFont(NotoSansBold15);

  tft.fillScreen(TFT_BLACK);

  gui_init();
  gui_set_input((int)1);
  
  //nfc.addNotifyAudioChange(i2s);
  nfc.begin(16, 32);

  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = 32;
  config.is_master = true;

  i2s.begin(config);
  dec.begin();
  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
}

void loop(){
  copier.copy();
  gui_update_task(nullptr);
}