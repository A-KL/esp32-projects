#pragma once

#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioLibs/SPDIFOutput.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include "VuOutput.h"

//                                                                   -> I2SStream
//                                                                   -> VuMeter
//                                                                   -> SPDIFOutput
//                            -> EncodedAudioStream -> MultiOutput -|                                                                  
// URLStream -> MultiOutput -|
//                            -> MetaDataOutput

// Output 
const int output_format = 16;
AudioInfo info(44100, 2, output_format);

I2SStream i2s;
SPDIFOutput spdif; 
VuMeter<int16_t> vu(0.1);

// Input
URLStream url(LOCAL_SSID, LOCAL_PASSWORD);

// Other
MultiOutput decoded_out;
MultiOutput encoded_out;

NumberFormatConverterStream nfc(decoded_out);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());

MetaDataOutput metadata;

StreamCopy copier(encoded_out, url); //, 2048

// Radio

const String urls[] = {
  "http://sunsl.streamabc.net/sunsl_w4pk5n9ou_0dfk-mp3-192-4232604",
  "http://stream.srg-ssr.ch/m/rsj/mp3_128",
  "http://radiospromo.com/listen/tracksaudio.com_-_old_school/radio.mp3"
};

static uint8_t urls_max = sizeof(urls) / sizeof(String);
static uint8_t current_station = 0;

// Methods

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

inline void audio_switch_out()
{
    if (i2s.isActive()) {
        i2s.end();
        //spdif.begin(spdif_config);
    } else {
        spdif.end();
        //i2s.begin(i2s_config);
    }
}

inline void audio_init()
{
//nfc.addNotifyAudioChange(i2s);
  nfc.begin(16, output_format);

  // S/PDIF
  auto spdif_config = spdif.defaultConfig();
  spdif_config.copyFrom(info); 
  spdif_config.pin_data = 23;
  spdif_config.buffer_size = 384;
  spdif_config.buffer_count = 8;

  //spdif.begin(spdif_config);

  // I2S
  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = output_format;
  config.is_master = true;

  encoded_out.add(metadata);
  encoded_out.add(decoder);

  decoded_out.add(vu);
  decoded_out.add(i2s);
  decoded_out.add(spdif);

  metadata.setCallback(print_metadata);

  i2s.begin(config);
  decoder.begin();
}

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

inline void audio_loop()
{
    copier.copy();
    // left_pb.value = vu.value_left();
    // right_pb.value = vu.value_right();
}