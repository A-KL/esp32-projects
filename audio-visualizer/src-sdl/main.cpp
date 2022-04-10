#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>

#include "../lib/BaseGraphics/Color.h"
#include "../lib/BaseGraphics/Canvas.h"
#include "../lib/SDLGraphics/SDLCanvas.h"

#include "../lib/UI/UIElement.h"
#include "../lib/UI/UILabel.h"
#include "../lib/UI/UIContainer.h"
#include "../lib/UI/UISoundAnalyzer.h"
#include "../lib/UI/UVProgress.h"

using namespace std;

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	TTF_Init();

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

	auto font = "C:/Windows/Fonts/Carlito-Regular.ttf";

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

	UILabel label_out_spdif({ 30, 0, 40, 18 }, "COAX", { 255, 0, 0, 0 }, 2);
	label_out_spdif.setForecolor({ 255, 0, 0, 0 });

	UISoundAnalyzer<30> analyzer({ 30, 25, 270, 120 });

	analyzer.setBand(0, 50);
	analyzer.setBand(1, 30);
	analyzer.setBand(2, 0);
	analyzer.setBand(3, 10);

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
	panel.Add(label_out_spdif);
	panel.Add(label_0);
	panel.Add(label_10);
	panel.Add(label_20);
	panel.Add(label_30);
	panel.Add(label_40);
	panel.Add(label_50);
	panel.Add(label_60);
	panel.Add(analyzer);
	panel.Add(level_left);
	panel.Add(level_right);
	panel.Add(level_left_label);
	panel.Add(level_right_label);
	panel.Add(footer);

	do
	{
		auto value_l = rand() % 255;
		auto value_r = rand() % 255;

		level_left.SetValueOf(value_l);
		level_right.SetValueOf(value_r);

		while (!level_left.IsValid() || !level_right.IsValid())
		{
			level_left.Update(sdl);
			level_right.Update(sdl);

			sdl.Update();

			SDL_PollEvent(&event);
			SDL_Delay(2);
		}

		level_left.SetValueOf(0);
		level_right.SetValueOf(0);

		while (!level_left.IsValid() || !level_right.IsValid())
		{
			level_left.Update(sdl);
			level_right.Update(sdl);

			sdl.Update();

			SDL_PollEvent(&event);
			SDL_Delay(7);
		}

		for (auto i = 0; i < 30; i++)
		{
			analyzer.setBand(i, (rand() % 255));
		}

		panel.Update(sdl);

		sdl.Update();

		SDL_PollEvent(&event);
		SDL_Delay(5);
	} 
	while (event.type != SDL_QUIT);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}