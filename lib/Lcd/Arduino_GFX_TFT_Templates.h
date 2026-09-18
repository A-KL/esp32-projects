#pragma once

  // template <typename TDisplay>
  class TFT_Arduino_GFX_Display
  {
    public: 
      TFT_Arduino_GFX_Display(Arduino_TFT* display) 
        : _display(display)
      {}

      bool init() { 
        return _display->begin();
      }

      bool isEPD() const { 
        return false;
      }

      void setEpdMode(int d) const
      {

      }

      void setSwapBytes(bool swap)
      {

      }

      void setColorDepth(uint8_t bpp)
      {

      }

      void initDMA()
      {

      }

      void loadFont(const uint8_t*) {

      }

      int fontHeight() {
        return 1; // _display->gfxFont->glyph->height;
      }

      void pushImage(int x, int y, int w, int h, const unsigned short* data) {
        _display->draw16bitRGBBitmap(x, y, (uint16_t*)data, w, h);
      }

      void drawCentreString(const char* text, int x, int y)
      {
        String s(text);
        _display->printCenterText(s, x, y);
      }

      void setTextColor(uint16_t color, uint16_t bg) {
        _display->setTextColor(color, bg);
      }

      void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
        _display->drawRect(x, y, w, h, color);
      }

      void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
        _display->fillRect(x, y, w, h, color);
      }

      void drawFastHLine(int16_t x, int16_t y,
                         int16_t w, uint16_t color)
      {
        _display->drawFastHLine(x, y, w, color);
      }

      void drawFastVLine(int16_t x, int16_t y,
                         int16_t h, uint16_t color)
      {
        _display->drawFastVLine(x, y, h, color);
      }

      void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    uint16_t color)
      {
        _display->drawLine(x0, y0, x1, y1, color);
      }

      void fillScreen(uint16_t color) {
        _display->fillScreen(color);
      }

      void setRotation(uint8_t r) {
        _display->setRotation(r);
      }

      uint8_t getRotation() const {
        return _display->getRotation();
      }

      void startWrite() {
        _display->startWrite();
      }

        void endWrite() {
        _display->endWrite();
      }

      uint16_t width() const {
        return _display->width();
      }

      uint16_t height() const {
        return _display->height();
      }

    inline Arduino_TFT* getTFT() const {
      return _display;
    }

    private:
      Arduino_TFT* _display;
  };

  class TFT_Arduino_Canvas_eSprite //: public Arduino_Canvas
  {
    public: 
      TFT_Arduino_Canvas_eSprite(TFT_Arduino_GFX_Display* display) 
      : _canvas(nullptr), _tft(display->getTFT())
      {}

      bool init() {
        return true;
      }

      void createSprite(int16_t w, int16_t h){
        _canvas = new Arduino_Canvas(w, h, _tft);
      }

      void deleteSprite() {
          delete _canvas;
          _canvas = nullptr;
      }

      void pushSprite(int16_t x, int16_t y) {
        _canvas->flush();
      }

      void drawFastHLine(int16_t x, int16_t y,
                         int16_t w, uint16_t color)
      {
        _canvas->drawFastHLine(x, y, w, color);
      }

      void drawFastVLine(int16_t x, int16_t y,
                         int16_t h, uint16_t color)
      {
        _canvas->drawFastVLine(x, y, h, color);
      }

      void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                    uint16_t color)
      {
        _canvas->drawLine(x0, y0, x1, y1, color);
      }

    private:
      Arduino_Canvas* _canvas;
      Arduino_TFT* _tft;
  };
