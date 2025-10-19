#pragma once

void hal_init() {
}

void hal_buttons_init() {
}

void hal_audio_init() {
}

void hal_gui_start() {
  gui_progress_bars_update();
}

void update_analog(){
}

void update_i2s(){
  left_pb.value = std::rand() % 1200;
  right_pb.value = std::rand() % 1200;
  SDL_Delay(400);
  gui_progress_bars_update();
}