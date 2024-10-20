#include <stdio.h>
#include <cstdint>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int main(void)
{
  tft.init();
  //tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);

  printf("fillRect");
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(10, 10, 100, 100, TFT_DARKCYAN);

  tft.drawFastHLine(110, 10, 100, TFT_DARKGREEN);

  tft.fillTriangle(10, 100, 10, 200, 200, 200, TFT_YELLOW);

  tft.fillRectHGradient(200, 10, 50, 50, TFT_RED, TFT_GREEN);

  tft.fillCircle(200, 100, 20, TFT_BLUE);

  tft.fillSmoothRoundRect(200, 100, 80, 30, 20, TFT_GOLD);

  TFT_eSprite sprite(&tft);

  sprite.setColorDepth(16);
  sprite.createSprite(100, 120);
  sprite.setSwapBytes(true);

  sprite.fillRect(10, 10, 50, 100, TFT_MAGENTA);
  //sprite.fillCircle(50, 160, 40, TFT_DARKGREEN);

  sprite.pushSprite(100, 100);

  sprite.deleteSprite();

  uint16_t x = 0, y = 0;

  bool pressed = tft.getTouch(&x, &y);

  tft.destroy();
}
