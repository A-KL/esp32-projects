#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioSourceSD.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h" // or AudioKissFFT or others

#include "gui.h"

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
I2SStream i2s;
AudioRealFFT fft;
MultiOutput decoded_out;
NumberFormatConverterStream nfc(decoded_out);
EncodedAudioStream decoder(&nfc, new MP3DecoderHelix());
StreamCopy copier(decoder, url);

const uint8_t output_bpp = 16;
const uint16_t output_format = 16;

void fftResult(AudioFFTBase &fft){
    float diff;
    auto result = fft.result();
    if (result.bin < 11)
    {
      spectrum.set_value(result.bin-1, map(result.magnitude, 0, 2000000, 0, 255));
    }
    // if (result.magnitude>100){
    //     Serial.print(result.bin);
    //     Serial.print(" ");
    //     Serial.print(result.frequency);
    //     Serial.print(" ");
    //     Serial.print(result.magnitude);  
    //     Serial.print(" => ");
    //     Serial.print(result.frequencyAsNote(diff));
    //     Serial.print( " diff: ");
    //     Serial.println(diff);
    // }
}

void setup()
{
  Serial.begin(115200);

  gui_init();

  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  nfc.begin(output_bpp, output_format);

  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = output_bpp;
  i2s.begin(config);

  auto tcfg = fft.defaultConfig();
  tcfg.copyFrom(config);
  tcfg.length = 1024;
  tcfg.bits_per_sample = output_bpp;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);

  decoded_out.add(fft);
  decoded_out.add(i2s);

  decoder.begin();

  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");

  gui_run(0);
}

void loop(){
  copier.copy();
}