#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"

//                            -> EncodedAudioStream -> I2SStream
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
MultiOutput encoded_out;
I2SStream i2s;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());
MetaDataOutput metadata;
StreamCopy copier(encoded_out, url);

const int output_format = 16;

void print_metadata(MetaDataType type, const char* str, int len){
  Serial.printf("==> %s: %s\r\n", toStr(type), str);
}

void setup(){
  Serial.begin(115200);

  gui_init();
  
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  //nfc.addNotifyAudioChange(i2s);
  nfc.begin(16, output_format);

  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = output_format;
  config.is_master = true;

  encoded_out.add(metadata);
  encoded_out.add(decoder);

  i2s.begin(config);
  decoder.begin();

  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");

  url.end();

  metadata.setCallback(print_metadata);
  metadata.begin(url.httpRequest());
//  gui_run(0);
}

void stop()
{
  url.end();
  metadata.end();
}

void loop(){
  copier.copy();

  // left_pb.value = vu.value_left();
  // right_pb.value = vu.value_right();
}