#include <OneButton.h>
#include <AiEsp32RotaryEncoder.h>

#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include "VuOutput.h"

// AiEsp32RotaryEncoder encoder_left = AiEsp32RotaryEncoder(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
// AiEsp32RotaryEncoder encoder_right = AiEsp32RotaryEncoder(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);
// OneButton left_button = OneButton(PIN_BUTTON_1, true, true);
// OneButton right_button = OneButton(PIN_BUTTON_2, true, true);

//                                                                    -> I2SStream
//                            -> EncodedAudioStream -> MultiOutput -|
//                                                                    -> VuMeter
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);

MultiOutput decoded_out;
MultiOutput encoded_out;

I2SStream i2s;
VuMeter<int16_t> vu(0.1);
NumberFormatConverterStream nfc(decoded_out);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());

MetaDataOutput metadata; // final output of metadata

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
  // left_button.attachClick([]() { stop_playing(); });
  // right_button.attachClick([]() { next_station(); });
}

void print_metadata(MetaDataType type, const char* str, int len){
  Serial.printf("==> %s: %s\r\n", toStr(type), str);

  switch (type)
  {
    case Description:
     // cassette.set_header(str);
      break;
    case Genre:
      //cassette.set_footer(str);
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
  // pinMode(PIN_LED, OUTPUT);
  // digitalWrite(PIN_LED, LOW);

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

  // left_button.tick();
  // right_button.tick();
}