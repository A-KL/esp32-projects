#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
I2SStream i2s;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream dec(&nfc, new MP3DecoderHelix());
StreamCopy copier(dec, url);

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  
  //nfc.addNotifyAudioChange(i2s);
  // convert 16 bits to 32, you could also change the gain
  nfc.begin(16, I2S_BPS);
  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  // you could define e.g your pins and change other settings
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = I2S_BPS;

  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  dec.begin();

// mp3 radio
  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
}

void loop(){
  copier.copy();
}