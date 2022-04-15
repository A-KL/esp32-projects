
InternetRadio radio;

void onStreamChanged(const char *type, const char *value)
{
  if (strcmp(type, "StreamTitle") == 0) {
    form.track.setText(value);
    Serial.println(value);
  }
}

void setupAudio(int dest, int src)
{
  radio.selectStation(Stations[2]);
  radio.Play(dest, src);
  radio.SampleCallback(onAudioFrameCallback);
  radio.StreamChanged = onStreamChanged;
}

void loopAudio()
{
    radio.Loop();
}