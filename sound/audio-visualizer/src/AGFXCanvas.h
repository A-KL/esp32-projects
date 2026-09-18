#pragma once

#include <Color.h>
#include <Canvas.h>

template<typename TDisplay>
class BaseCanvas : public Canvas<Color>
{
  public:
    BaseCanvas(TDisplay* display) : _display(display), _background(Color::Black)
    {}

    /* Draw API */

    void DrawLine(int x0, int y0, int x1, int y1, const Color& color) {
      if (y0 == y1) {
        _display->drawFastHLine(x0, y0, x1 - x0, (unsigned short)color);
      } else if (x0 == x1) {
        _display->drawFastVLine(x0, y0, y1 - y0, (unsigned short)color);
      } else {
        _display->drawLine(x0, y0, x1, y1, (unsigned short)color);
      }
    }

    void DrawFilledRect(int x0, int y0, int w, int h, const Color& color) {
      _display->fillRect(x0, y0, w, h, (unsigned short)color);
    }

    void DrawRect(int x0, int y0, int w, int h, const Color& color) {
      _display->drawRect(x0, y0, w, h, (unsigned short)color);
    }

  	void DrawText(int x, int y, int w, int h, const char* text, const Color& color) {
      _display->setTextColor((unsigned short)color, TFT_BLACK);
     // _display->drawCentreString(text, x + w/2, y + (h - _display->fontHeight() + 2) / 2);
	  }

    /* General */

    inline int Height() const {
      return _display->height();
    }

    inline int Width() const {
      return _display->width();
    }

    inline unsigned int Bpp() const {
      return 16;
    }

    inline void Clear(const Color& color) {
      _display->fillScreen((unsigned short)color);
    }

    inline void Clear() {
      Clear(_background);
    }

  protected:
    TDisplay* _display;
    Color _background;
};

template<typename TDisplay>
class AGFXCanvas : public BaseCanvas<TDisplay>
{
  public:
    AGFXCanvas(TDisplay* display) : BaseCanvas<TDisplay>(display), _sprite(nullptr)
    {}

    bool Init(const Color& color = Color::Black)
    {
      BaseCanvas<TDisplay>::_display->init(TFT_WIDTH, TFT_HEIGHT);
      BaseCanvas<TDisplay>::_display->invertDisplay(false);
      // if (!_display->init(TFT_WIDTH, TFT_HEIGHT))
      // {
      //   log_e("TFT ERROR");
      //   return false;
      // }

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
      BaseCanvas<TDisplay>::_display->drawRGBBitmap(x, y, data, w, h);
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
      _sprite = new GFXcanvas16(w, h);
      assert (_sprite->getBuffer() != 0);

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
      BaseCanvas<TDisplay>::_display->drawRGBBitmap(x, y, _sprite->getBuffer(), _sprite->width(), _sprite->height());
    }

  private: 
    GFXcanvas16 *_sprite;
};