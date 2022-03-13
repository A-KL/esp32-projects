#pragma once

void run_ui(void * args)
{
    auto canvas = *(TFTCanvas*)args; //Canvas<Color>

    // UI
    UIContainer panel({ 0, 0, 320, 240 });

    // Title
    const char* font = NULL;
    UILabel label({ 0, 0, 320, 25 }, "S/PDIF", font, 16);

    // Main
    auto start = 18;
    UILabel label_0({ 8, start, 18, 16 }, "0", font, 16);
    UILabel label_10({ 5, start += 19, 20, 16 }, "-10", font, 16);
    UILabel label_20({ 5, start += 20, 20, 16 }, "-20", font, 16);
    UILabel label_30({ 5, start += 19, 20, 16 }, "-30", font, 16);
    UILabel label_40({ 5, start += 20, 20, 16 }, "-40", font, 16);
    UILabel label_50({ 5, start += 19, 20, 16 }, "-50", font, 16);
    UILabel label_60({ 5, start += 20, 20, 16 }, "-60", font, 16);

	UISoundAnalyzer<BANDS_COUNT> analyzer({ 30, 25, 270, 120 });

    // Levels
    UILabel level_left_label({ 0, 181, 20, 16 }, "L", NULL, 16);
    UILabel level_right_label({ 0, 181 + 13 + 3, 20, 16 }, "R", NULL, 16);

    UVProgress<uint8_t> level_left({ 24, 181,           246, 15 }, 0, 4095, 4095 * 0.9, (uint8_t)0);
    UVProgress<uint8_t> level_right({ 24, 181 + 15 + 3, 246, 15 }, 0, 4095, 4095 * 0.9, (uint8_t)0);

    level_left.Clear(canvas);
    level_right.Clear(canvas);

    // Footer
    UIContainer footer({ 0, 240-23, 320, 23 });
    footer.Background = { 56, 56, 56, 0 };
    footer.Clear(canvas);

    // Build UI
	panel.Add(label);
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

    while (true)
    {
        panel.Draw(canvas);

        vTaskDelay(100);

    }

    vTaskDelete(NULL);
}