#include "AudioTools.h"
#include "AudioTools/AudioLibs/VBANStream.h"

AudioInfo info(44100, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
VBANStream out;
StreamCopy copier(out, sound, 2048);                       // 44100 needs 2048

// Arduino Setup
void setup(void) {  
    // Open Serial 
    Serial.begin(115200);
    while(!Serial);
    AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
  
    // setup output
    auto cfg = out.defaultConfig(TX_MODE);
    cfg.copyFrom(info);
    cfg.ssid = "";
    cfg.password = "";
    cfg.stream_name = "Stream1";
    cfg.target_ip = IPAddress{192,168,1,30}; 
    cfg.throttle_active = true;
    //cfg.throttle_correction_us = 0; // optimize overload and underrun
    if (!out.begin(cfg)) stop();
  
    // Setup sine wave
    sineWave.begin(info, N_B4);
    Serial.println("started...");
  }
  
  // Arduino loop - copy sound to out 
  void loop() {
    copier.copy();
  }