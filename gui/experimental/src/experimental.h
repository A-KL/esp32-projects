#pragma once

#include "NotoSansBold36.h"

void fillChevronRect_v2(
  LGFX_Sprite* sprite, 
  const int32_t x, const int32_t y, 
  const int32_t w, const int32_t h, 
  int _background_color, int _chevron_color, 
  int16_t chevron_width = -1, int16_t left = 0)
{
    sprite->fillRect(x, y, w, h, _background_color);

    auto ch_w = chevron_width > 0 ? chevron_width : h;
    auto left_x = left - ch_w * (h/ch_w);
    
    auto ch_w_actual = ch_w / 2;
    auto ch_w_step = ch_w * 3;

    auto x0 = x - ch_w;
    auto y0 = y - ch_w;

    auto x1 = x + ch_w + h + ch_w;
    auto y1 = y + h + ch_w;

    while (left_x < w)
    {
        sprite->drawWideLine(
            left_x + x0, y0,
            left_x + x1, y1, 
            ch_w_actual,
            _chevron_color);

        left_x += ch_w_step;
    }
}

void apply_background_color(LGFX_Sprite& spt, const int color)
{
  spt.fillSprite(color);
}

void apply_pattern(LGFX_Sprite& spt, const int16_t w, const int16_t h, const int cut)
{
  spt.fillTriangle(0, h-cut, cut, h, 0, h, TFT_TRANSPARENT);
}

struct lgfx_card_style_t
{
  int16_t x =0;
  int16_t y = 0;

  int16_t w = 200;
  int16_t h = 100;

  const char* main_text;
  const char* second_text;

  int bg_color = TFT_DARKGRAY;
  int lower_bg_color = TFT_YELLOW;

  int border = 4; //3
};

void test_card_v2(LGFX& tft, const char* main_text, const char* second_text, int16_t x, int16_t y, int16_t w, int16_t h)
{
  auto spr = LGFX_Sprite(&tft);

  auto bg_color = tft.color24to16(0x46E074);
  auto second_bg_color = tft.color24to16(0x202020);

  auto background = tft.color24to16(0x404040); //tft.color24to16(0x770000); //
  auto foreground = tft.color24to16(0x303030); // tft.color24to16(0x440000); //
  auto border = 3;
  auto bottom_panel_h = h * 0.2;

  // Part 1
  spr.createSprite(w, h);

  //fillChevronRect_v2(&spr, 0, 0, w, h, background, foreground, 15);
  apply_background_color(spr, bg_color);
  apply_pattern(spr, w, h, 13);

  spr.pushSprite(x, y, TFT_TRANSPARENT);
  spr.deleteSprite();

  // part 2
  spr.createSprite(w - border * 2, bottom_panel_h);

  apply_background_color(spr, second_bg_color);   
  apply_pattern(spr, w - border * 2, bottom_panel_h, 11); 

  spr.pushSprite(x + border, y + h - bottom_panel_h - border,  TFT_TRANSPARENT);
  spr.deleteSprite();
}

void test_card(LGFX& tft, const char* main_text, const char* second_text, int16_t x, int16_t y, int16_t w)
{
  auto spr = LGFX_Sprite(&tft);

  auto background = tft.color24to16(0x404040); // tft.color24to16(0x69DD7B);
  auto background_panel = tft.color24to16(0x303030); // tft.color24to16(0x69DD7B);
  
  auto h = w * 0.7;
  auto border = 3;
  auto bottom_panel_h = h * 0.2;
  auto cut = 18;

  spr.createSprite(w, h);
  spr.loadFont(NotoSansBold36);

  spr.fillSprite(background);
  spr.fillRect(border, h - bottom_panel_h - border, w - border * 2, bottom_panel_h, background_panel);

  // spr.setTextColor(TFT_BLACK);
  // spr.drawString(main_text, w - 70, (h-spr.fontHeight() + 6)/2);

  spr.fillTriangle(0, h-cut - border, cut + border, h, 0, h, background);
  spr.fillTriangle(0, h-cut, cut, h, 0, h, TFT_TRANSPARENT);

  spr.pushSprite(x, y, TFT_TRANSPARENT);
  spr.deleteSprite();
}

void test_chevron(LGFX& tft, int x, int y, int w, int h)
{
  auto img = LGFX_Sprite(&tft);
  
  img.createSprite(w, h);
  img.loadFont(NotoSansBold36);
  // img.setColorDepth(16);
  // img.setSwapBytes(true);

  auto background = tft.color24to16(0x404040); //tft.color24to16(0x770000); //
  auto foreground = tft.color24to16(0x303030); // tft.color24to16(0x440000); //

  auto text_color = TFT_YELLOW; //TFT_RED; 
  auto mark_padding = 5;

  fillChevronRect_v2(&img, 0, 0, w, h, background, foreground, 15);

  img.fillTriangle(w, h-20, w, h, w-20, h, TFT_TRANSPARENT);

  img.fillRect(mark_padding, mark_padding, 7, h-mark_padding*2, text_color);
  img.setTextColor(text_color);
  img.drawString("Warning", 25, (h-img.fontHeight() + 6)/2);

  img.pushSprite(x, y, TFT_TRANSPARENT);
  img.deleteSprite();
}