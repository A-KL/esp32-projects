#pragma once

#define INDICATOR_WIDTH   15
#define INDICATOR_PADDING 40

void ui_draw_background(TFT_eSPI &tft)
{
  tft.drawRect(5, 5,  tft.width() - 5 * 2, INDICATOR_WIDTH, Color::White);
  tft.drawRect(5, 5 + INDICATOR_PADDING,  tft.width() - 5 * 2, INDICATOR_WIDTH, Color::White);

  tft.drawLine(tft.width()/2, 5 + 2, tft.width()/2, INDICATOR_WIDTH + 2, Color::White);
  tft.drawLine(tft.width()/2, 5 + 2 + INDICATOR_PADDING, tft.width()/2, INDICATOR_WIDTH + 2 + INDICATOR_PADDING, Color::White);
}

void ui_draw_values(TFT_eSPI &tft)
{
  tft.fillRect(6, 6,  tft.width() - 5*2 -2, INDICATOR_WIDTH-2, Color::Black);
  tft.fillRect(6, 6 + INDICATOR_PADDING,  tft.width()-5 * 2-2, INDICATOR_WIDTH-2, Color::Black);

  tft.drawCentreString("100%", tft.width()/2, tft.height()/2-2);
}