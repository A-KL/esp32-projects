#pragma once

void onAudioFrameCallback(const AudioFrame& frame)
{
  xQueueSend(audioFrameQueue, &frame, 0);
}

void onStreamChanged(const char *type, const char *value)
{
  if (strcmp(type, "StreamTitle") == 0) {
    form.track.setText(value);
    Serial.println(value);
  }
}

void setupAudio(int dest, int src)
{
  switch (src)
  {
    case 0:
      radio.selectStation(Stations[2]);
      radio.Play(dest, src);
      radio.SampleCallback(onAudioFrameCallback);
      radio.StreamChanged = onStreamChanged;
      break;

    case 1:
      break;
    
    default:
      break;
  }
}

void loopAudio()
{
    if (_selectedAudioSource == 1)
    {
      adc.loop();
    } 
    else if (_selectedAudioSource == 0)
    {
      radio.Loop();
    }  
}