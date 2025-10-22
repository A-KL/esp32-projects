#include "AudioTools.h"
#include "AudioTools/Disk/AudioSourceSTD.h"

#include "AudioTools/AudioCodecs/CodecAACHelix.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "AudioTools/AudioLibs/StdioStream.h"
#include "AudioTools/AudioLibs/PortAudioStream.h"


AudioInfo info(44100, 2, 16);
PortAudioStream out;
AudioSourceSTD source("/Users/anatolii.klots/Documents/Sources/esp32-projects/sound/audio-visualizer/sound/", ".aac");

AACDecoderHelix decoder;
//MP3DecoderHelix decoder;

AudioPlayer player(source, out, decoder);

int main(int, char**)
{
   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

  auto cfg = out.defaultConfig();
  cfg.copyFrom(info);
  out.begin(cfg);

  player.begin();
  source.begin();

  while (true) {
    player.copy();
  }

  return 0;
}