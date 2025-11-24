#pragma once

void setVolume(float value) 
{
  //log_e("Volume: %f", value);
  static bool is_muted;
  static int volume;

  // Set MUTED
  if (!is_muted && value == 0) 
  {
    form.volume.setForecolor(Color::Gray);
    form.setIcon(5, true);
    
    volume_out.setVolume(0);
    is_muted = true;
    return;
  }
  
  if (is_muted)
  { 
    if (value == 0) {
      // Remove MUTED
      form.volume.setForecolor(Color::White);
      form.setIcon(5, false);
      is_muted = false;
      // Turn ON volume
      volume_out.setVolume(volume);
    } else {
      // Update UI
      auto dbs = (int)(value * 255 - 127.5);
      form.volume.setTextF("%ddb", dbs);
      volume = value;
    }
  } 
  else 
  {
      // Update UI
      auto dbs = (int)(value* 255 - 127.5);
      form.volume.setTextF("%ddb", dbs);
      volume = value;
      // Turn ON volume
      volume_out.setVolume(volume);
  }
    // volumeDac(value);
}

void changeAudioInput()
{
  // inputs[selected_input]->end();
  // selected_input++;

  // if (inputs_count <= selected_input) {
  //   selected_input = 0;
  // }
  // inputs[selected_input]->begin();
}

void selectAudio(int dest, int src) 
{
  // static auto _selectedAudioSource = 0;
  // static auto _selectedAudioTarget = 1;
  // selectAudio(_selectedAudioTarget, _selectedAudioSource);
  // form.setIcon(_selectedAudioTarget, 1);
  // form.setIcon(_selectedAudioSource + 2, 1);
}