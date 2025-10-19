#pragma once

void setupInit()
{
  
}

void startUI(void* args)
{
  auto canvas = *(LovyanGFXCanvas*)args;

  form.levelLeft.setValueOf(100);
  form.levelRight.setValueOf(150);

  form.Update(canvas);
}

void setupWiFi()
{
  lgfx::delay(1000);
}

void setupControls()
{
  lgfx::delay(1000);
}

void setupAudio()
{
  lgfx::delay(1000);
}


void selectAudio(int dest, int src)
{

}

void loopControls()
{
  
}

void loopAudio()
{

}