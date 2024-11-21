#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include <OneButton.h>

//                            -> EncodedAudioStream -> I2SStream
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

OneButton left_button = OneButton(PIN_BUTTON_1, true, true);
OneButton right_button = OneButton(PIN_BUTTON_2, true, true);

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
MultiOutput encoded_out;
I2SStream i2s;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());
MetaDataOutput metadata;
StreamCopy copier(encoded_out, url);

const int output_format = 16;

const String urls[] = {
  "http://sunsl.streamabc.net/sunsl_w4pk5n9ou_0dfk-mp3-192-4232604",
  "http://stream.srg-ssr.ch/m/rsj/mp3_128",
  "http://radiospromo.com/listen/tracksaudio.com_-_old_school/radio.mp3"
};

const uint8_t urls_max = sizeof(urls) / sizeof(String);

static uint8_t current_station = 0;

void start_playing()
{
  url.begin(urls[current_station].c_str(), "audio/mp3");
  metadata.begin(url.httpRequest());
}

void stop_playing()
{
  url.end();
  metadata.end();

  angle_d = 0;
  digitalWrite(PIN_LED, LOW);
}

void next_station()
{
  stop_playing();

  if (current_station >= urls_max - 1) {
      current_station = 0;
  }
  else {
      current_station ++;
  }
  start_playing();
}

void buttons_init()
{
  left_button.attachClick([]() { stop_playing(); });
  right_button.attachClick([]() { next_station(); });
}

void print_metadata(MetaDataType type, const char* str, int len){
  Serial.printf("==> %s: %s\r\n", toStr(type), str);

  //angle_d = 3;
  digitalWrite(PIN_LED, HIGH);

  switch (type)
  {
    case Description:
      cassette.set_header(str);
      break;
    case Genre:
      cassette.set_footer(str);
      break;
    default:
      break;
  }
}

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  gui_init();
  buttons_init();
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

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

  metadata.setCallback(print_metadata);

  gui_run(0);

  start_playing();
}

void loop(){
  copier.copy();

  left_button.tick();
  right_button.tick();
}