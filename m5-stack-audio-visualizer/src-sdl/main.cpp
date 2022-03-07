#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

#include "../lib/BaseGraphics/Color.h"
#include "../lib/BaseGraphics/Canvas.h"
#include "../lib/SDLGraphics/SDLCanvas.h"

#include "../lib/UI/UI.h"

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
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

	UILabel label({ 0, 0, 320, 25 }, "S/PDIF", font, 20);

	// Main

	UIRect rect3{ 30, 25, 270, 120 };
	UISoundAnalyzer<30> analyzer(rect3);

	// Levels

	UILabel level_left_label({ 0, 181, 24, 13 }, "L", font, 16);
	UILabel level_right_label({ 0, 181 + 13 + 3, 24, 13 }, "R", font, 16);

	UVProgress<Uint8> level_left({ 24, 181, 244, 13 }, 0, 255, 200, 110);
	UVProgress<Uint8> level_right({ 24, 181 + 13 + 3, 244, 13 }, 0, 255, 200, 50);

	// Footer

	UIContainer footer({ 0, 240-23, 320, 23 });
	footer.Background = { 56, 56, 56, 0 };

	// Render
	panel.Add(label);
	panel.Add(analyzer);
	panel.Add(level_left);
	panel.Add(level_right);
	panel.Add(level_left_label);
	panel.Add(level_right_label);
	panel.Add(footer);

	panel.Draw(sdl);

	do
	{
		auto value_l = rand() % 255;
		auto value_r = rand() % 255;

		auto animated_l = level_left.Get();
		auto animated_r = level_right.Get();

		auto d_l = (value_l - animated_l) / abs(value_l - animated_l);
		auto d_r = (value_r - animated_r) / abs(value_r - animated_r);

		while (true)
		{
			if (animated_l != value_l)
			{
				level_left.Set(animated_l);
				level_left.Draw(sdl);

				animated_l += d_l;
			}

			if (animated_r != value_r)
			{
				level_right.Set(animated_r);
				level_right.Draw(sdl);

				animated_r += d_r;
			}

			if (animated_l == value_l && animated_r == value_r)
			{
				break;
			}

			sdl.Update();

			SDL_PollEvent(&event);
			SDL_Delay(5);
		}

		SDL_PollEvent(&event);
		SDL_Delay(50);
	} 
	while (event.type != SDL_QUIT);


	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
