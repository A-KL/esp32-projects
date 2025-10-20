#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

#include "../lib/FFT_CODE/complex.h"
#include "../lib/FFT_CODE/fft.h"

#include "../lib/BaseGraphics/Color.h"
#include "../lib/BaseGraphics/Canvas.h"
#include "../lib/SDLGraphics/SDLCanvas.h"

#include "../lib/UI/UIElement.h"
#include "../lib/UI/UILabel.h"
#include "../lib/UI/UIContainer.h"
#include "../lib/UI/UISoundAnalyzer.h"
#include "../lib/UI/UVProgress.h"

#include "MainForm.h"


MainForm form({0, 0, 320, 240});
UISoundAnalyzer<30> analyzer({ 30, 25, 270, 120 });

#include "audio.h"

using namespace std;

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	if (TTF_Init() != 0)
	{
		SDL_Log("Unable to initialize TTF: %s", SDL_GetError());
		return 1;
	}

	SDL_Event event;

	SDL_Window* window = SDL_CreateWindow(
		"EQ Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		320, 240, 0);

	SDLCanvas sdl(window);

	// UI

	UIContainer panel({ 0, 0, 320, 240 });

	// Title

	//auto font = "/Users/anatolii.klots/Documents/Sources/esp32-projects/sound/audio-visualizer/src-sdl/Carlito/Carlito-Regular.ttf";

	UILabel label({ 0, 0, 320, 25 }, "S/PDIF");

	// Main

	auto start = 20;
	UILabel label_0({ 14, start, 24, 13 }, "0");
	UILabel label_10({ 17, start += 19, 24, 13 }, "-10");
	UILabel label_20({ 17, start += 20, 24, 13 }, "-20");
	UILabel label_30({ 17, start += 19, 24, 13 }, "-30");
	UILabel label_40({ 17, start += 20, 24, 13 }, "-40");
	UILabel label_50({ 17, start += 19, 24, 13 }, "-50");
	UILabel label_60({ 17, start += 20, 24, 13 }, "-60");

	UILabel label_out_spdif({ 30, 0, 40, 18 }, "COAX", Color::Red, Color::Red, 2);
	label_out_spdif.setForecolor({ 255, 0, 0, 0 });

	//analyzer.setBand(0, 50);
	//analyzer.setBand(1, 30);
	//analyzer.setBand(2, 0);
	//analyzer.setBand(3, 10);

	// Levels

	UILabel level_left_label({ 0, 181, 24, 13 }, "L");
	UILabel level_right_label({ 0, 181 + 13 + 3, 24, 13 }, "R");

	UVAnimatedProgressOf<Uint8> level_left({ 24, 181, 246, 13 }, 0, 255, 200, 0);
	UVAnimatedProgressOf<Uint8> level_right({ 24, 181 + 13 + 3, 246, 13 }, 0, 255, 200, 0);

	level_left.Clear(sdl);
	level_right.Clear(sdl);

	// Footer

	UIContainer footer({ 0, 240-23, 320, 23 }, { 56, 56, 56, 0 });

	// Render
	//panel.Add(label_0);
	//panel.Add(label_10);
	//panel.Add(label_20);
	//panel.Add(label_30);
	//panel.Add(label_40);
	//panel.Add(label_50);
	//panel.Add(label_60);
	panel.Add(analyzer);
	panel.Add(level_left);
	panel.Add(level_right);
	panel.Add(level_left_label);
	panel.Add(level_right_label);

	listDevices(0);

	auto dev = setupAudio("/esp32-projects/sound/audio-visualizer/src-sdl/BabyElephantWalk60.wav");

	SDL_PauseAudioDevice(dev, SDL_FALSE); /* start audio playing. */

	do
	{
		//auto value_l = rand() % 255;
		//auto value_r = rand() % 255;

		//level_left.SetValueOf(value_l);
		//level_right.SetValueOf(value_r);

		//while (!level_left.IsValid() || !level_right.IsValid())
		//{
		//	panel.Update(sdl);

		//	sdl.Update();

		//	SDL_PollEvent(&event);
		//	SDL_Delay(2);
		//}

		//form.Update(sdl);

		//for (auto i = 0; i < 30; i++)
		//{
		//	analyzer.setBand(i, (rand() % 255));
		//}

		panel.Update(sdl);
		sdl.Update();

		SDL_PollEvent(&event);
		SDL_Delay(5);
	} 
	while (event.type != SDL_QUIT);

	SDL_CloseAudioDevice(dev);

	SDL_FreeWAV(wavBuffer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
