#include "AudioTools.h"
#include "AudioTools/Disk/AudioSourceSTD.h"

#include "AudioTools/AudioCodecs/CodecAACHelix.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "AudioTools/AudioLibs/PortAudioStream.h"

PortAudioStream out;
AudioSourceSTD source("./sound/", ".aac");

AACDecoderHelix decoder;
//MP3DecoderHelix decoder;

AudioPlayer player(source, out, decoder);

int main()
{
   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

  auto cfg = out.defaultConfig();
  out.begin(cfg);

  player.begin();
  source.begin();

  while (true) {
    player.copy();
  }

  return 0;
}