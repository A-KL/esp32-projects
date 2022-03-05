
#include <list>
#include <M5GFX.h>

#include "Color.h"
#include "Canvas.h"
#include "M5StackCanvas.h"

#include "UI.h"

M5GFX display;
M5StackCanvas canvas(display);

void run(Canvas<Color>& canvas)
{
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

	UVProgress<unsigned char> level_left({ 24, 181, 244, 13 }, 0, 255, 200, 110);
	UVProgress<unsigned char> level_right({ 24, 181 + 13 + 3, 244, 13 }, 0, 255, 200, 50);

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

	panel.Draw(canvas);
}

void setup(void)
{ 
    canvas.Init(Color(0, 0, 0));
	display.waitDisplay();

	run(canvas);
}

void loop(void)
{ 
  delay(10);
}