#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h" // or AudioKissFFT or others

#include "gui.h"

#ifdef ARDUINO
  #include "AudioTools/Communication/AudioHttp.h"

  URLStream in(LOCAL_SSID, LOCAL_PASSWORD);
  I2SStream out;
#else
  #include <iostream>
  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  PortAudioStream out;
  File in("./sound/file_example_MP3_700KB.mp3");
#endif

AudioInfo info(44100, 2, 16);
AudioRealFFT fft;
MultiOutput decoded_out;
// NumberFormatConverterStream nfc(decoded_out);
EncodedAudioStream decoder(&decoded_out, new MP3DecoderHelix());
StreamCopy copier(decoder, in);

const uint8_t output_bpp = 16;
const uint16_t output_format = 16;

void fftResult(AudioFFTBase &fft)
{
  for(auto i=0; i<spectrum.size(); i++)
  {
    spectrum.set_value(i, fft.magnitude((i+1) * 2)*2);
  }
}

void setup()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  gui_init();

  //nfc.begin(output_bpp, output_format);

#ifdef ARDUINO
  auto config = out.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = output_bpp;
#else
  auto config = out.defaultConfig();
  config.copyFrom(info);
#endif
  out.begin(config);

  auto tcfg = fft.defaultConfig();
  tcfg.copyFrom(config);
  tcfg.length = 1024;
  tcfg.bits_per_sample = output_bpp;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);

  decoded_out.add(fft);
  decoded_out.add(out);

  decoder.begin();

#ifdef ARDUINO
  in.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
#else
  in.begin();
#endif

  //gui_run();
}

void loop(){
  copier.copy();
  gui_update();
}