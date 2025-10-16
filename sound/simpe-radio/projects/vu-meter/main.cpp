#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include "VuOutput.h"

//                                                                    -> I2SStream
//                            -> EncodedAudioStream -> MultiOutput -|
//                                                                    -> VuMeter
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);

MultiOutput decoded_out;
MultiOutput encoded_out;

I2SStream i2s;
VuMeter<int16_t> vu(AUDIO_VU_RATIO);
NumberFormatConverterStream nfc(decoded_out);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());

MetaDataOutput metadata; // final output of metadata

StreamCopy copier(encoded_out, url);

void print_metadata(MetaDataType type, const char* str, int len){
  Serial.printf("==> %s: %s\r\n", toStr(type), str);
}

void setup(){
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  gui_init();
  gui_set_input((int)1);
  
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  //nfc.addNotifyAudioChange(i2s);
  nfc.begin(16, I2S_BPS);

  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");

  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.is_master = true;

  //encoded_out.add(metadata);
  encoded_out.add(decoder);

  decoded_out.add(vu);
  decoded_out.add(i2s);

  //metadata.setCallback(print_metadata);
 // metadata.begin(url.httpRequest());

  i2s.begin(config);
  decoder.begin();

  //gui_run(0);
}

void loop(){
  copier.copy();
  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();
  gui_progress_bars_update();
}