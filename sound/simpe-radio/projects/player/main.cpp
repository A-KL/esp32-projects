#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include <TFT_eSPI.h>
#include "TFT_eGUI/TFT_eGUI.h"

//                            -> EncodedAudioStream -> I2SStream
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);

MultiOutput encoded_out;

I2SStream i2s;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());

MetaDataOutput metadata; // final output of metadata

StreamCopy copier(encoded_out, url);

const int output_format = 16;

TFT_eSprite canvas = TFT_eSprite(&tft);
TFT_eCassettePlayer player(&canvas, TFT_WIDTH, TFT_HEIGHT, 0, 0);

void print_metadata(MetaDataType type, const char* str, int len){
  Serial.printf("==> %s: %s\r\n", toStr(type), str);
}

void setup(){
  Serial.begin(115200);

  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);

  // //tft.setFreeFont(&Orbitron_Medium_20);
  // tft.loadFont(NotoSansBold15);

  tft.fillScreen(TFT_RED);
  
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  //nfc.addNotifyAudioChange(i2s);
  nfc.begin(16, output_format);

  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");

  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = output_format;
  config.is_master = true;

  encoded_out.add(metadata);
  encoded_out.add(decoder);

  metadata.setCallback(print_metadata);
  metadata.begin(url.httpRequest());

  i2s.begin(config);
  decoder.begin();

//  gui_run(0);
}

void loop(){
  copier.copy();

  // left_pb.value = vu.value_left();
  // right_pb.value = vu.value_right();
}