#pragma once

#include <esp32-hal-log.h>

#include <Color.h>
#include <Canvas.h>

#include "BaseCanvas.h"
#include "Arduino_GFX_Library.h"

template<typename TDisplay>
class ArduinoGFXCanvas : public BaseCanvas<TDisplay>
{
  public:
    ArduinoGFXCanvas(TDisplay* display) 
    : BaseCanvas<TDisplay>(display), _sprite(nullptr)
    {}

    bool Init(const Color& color = Color::Black)
    {
      if (!BaseCanvas<TDisplay>::_display->begin())
      {
        log_e("TFT ERROR");
        return false;
      }

      if (BaseCanvas<TDisplay>::_display->width() < BaseCanvas<TDisplay>::_display->height())
      {
        BaseCanvas<TDisplay>::_display->setRotation(BaseCanvas<TDisplay>::_display->getRotation() ^ 1);
      }

      BaseCanvas<TDisplay>::_display->fillScreen((unsigned short)color);

      log_i("TFT OK");

      return true;
    }

    /* Draw API */

    void DrawImage(int x, int y, int w, int h, const unsigned short* data) {
      BaseCanvas<TDisplay>::_display->draw16bitRGBBitmap(x, y, data, w, h);
    }

    /* Fonts API */

    inline void SetFont(int index, unsigned char size)
    { 
      // _display->setFont();
    }

    inline void LoadFont(const uint8_t* array, const size_t size) {
      // _display->loadFont(array);
      //BaseCanvas<TDisplay>::_display->setFont(array);
    }

    inline void Update() {
      //_display->display();
    }

    /* Sprite */

    void SpriteBegin(int w, int h, const Color& color)
    {
      _sprite = new Arduino_Canvas(w, h, BaseCanvas<TDisplay>::_display);
      // assert (_sprite->getBuffer() != 0);

      //_sprite.setColorDepth(16);
      //_sprite.setSwapBytes(true);
      //_sprite.fillSprite((unsigned short)color);
      //log_w("sprite %d x %d created: %d", w, h, res != nullptr);
    }

    void SpriteEnd() {
      delete _sprite;
      _sprite = nullptr;
    }

    void SpriteDrawLine(int x0, int y0, int x1, int y1, const Color& color) {
      if (y0 == y1) {
        _sprite->drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
      } else if (x0 == x1) {
        _sprite->drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
      } else {
        _sprite->drawLine(x0, y0, x1, y1, (unsigned short)color);
      }
    }

    void SpritePush(int x, int y) 
    {
      BaseCanvas<TDisplay>::_display->draw16bitRGBBitmap(x, y, _sprite->_rowBuf, _sprite->width(), _sprite->height());
    }

  private: 
    Arduino_Canvas *_sprite;
};