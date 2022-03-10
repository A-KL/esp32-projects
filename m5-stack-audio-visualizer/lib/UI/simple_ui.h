
void render_analyzer_level_lables(M5GFX& display, int x, int y)
{
  //display.setTextPadding(display.textWidth("100"));
  auto step = 19;
  display.drawString("  0", x, y);
  display.drawString("-10", x, y += step);
  display.drawString("-20", x, y += step);
  display.drawString("-30", x, y += step);
  display.drawString("-40", x, y += step);
  display.drawString("-50", x, y += step);
  display.drawString("-60", x, y += step);
}

void render_analyzer_update(M5GFX& display, int x0, int y0, int w, int h, int bands, const uint8_t* values)
{
    auto colorDarkGreen = display.color888(9, 14, 7);
    auto colorGreen = display.color888(100, 199, 73);
    auto colorDarkYellow = display.color888(35, 35, 15 );
    auto colorYellow = display.color888(243, 232, 53);

	auto vertical_elements_count = 10 * 6;

    auto element_padding_x = 3;
    auto element_padding_y = 1;

    auto element_width = (w / bands) - element_padding_x;

    auto origin_x = x0;
    auto origin_y = y0;

    auto y = origin_y;

    for (int i = 0; i < vertical_elements_count; i++)
    {
        auto x = origin_x;

        auto color = (i % 10 == 0 || i == (vertical_elements_count - 1)) ? colorDarkYellow : colorDarkGreen;
        auto active_color = (i % 10 == 0 || i == (vertical_elements_count - 1)) ? colorYellow : colorGreen;
        auto main_color = color;

        for (int j = 0; j < bands; j++)
        {
            if ((vertical_elements_count - values[j]) < i)
            {
                main_color = active_color;
            }
            else
            {
                main_color = color;
            }

            x += element_padding_x;

            display.drawLine(x, y, x + element_width, y, main_color);

            x += element_width;
        }

        y += 1 + element_padding_y;
    }
}

void render_analyzer_ui(M5GFX& display, int x, int y, int w, int h, int bands, const uint8_t* values)
{
    display.fillRect(x, y, w, h, TFT_BLACK);

    render_analyzer_level_lables(display, x + 5, y + 20);

    render_analyzer_update(display, x + 30, y + 20, 270, 120, bands, values);
}