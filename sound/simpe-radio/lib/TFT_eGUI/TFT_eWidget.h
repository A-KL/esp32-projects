#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eWidget
{
    public:
        TFT_eWidget(TFT_eSPI* tft, int16_t width, int16_t height, int16_t top, int16_t left) 
        : _canvas(tft), _canvas_to_rotate(tft), width(width), height(height), left(left), top(top)
        { }

        int16_t width;
        int16_t height;
        int16_t top;
        int16_t left;

        virtual void init() = 0;

        virtual void begin() {
            update();
        }

        virtual void update() = 0;

        virtual void load_font(const uint8_t array[])
        {
            _canvas.loadFont(array);
        }

        void set_parent(TFT_eSprite* parent)
        {
            _parent = parent;
        }

    protected:
        TFT_eSprite _canvas;

        inline TFT_eSprite* create(int16_t w, int16_t h, uint32_t bg_color)
        {
            create_sprite(_canvas, w, h, bg_color);

            return  &_canvas;
        }

        inline void del()
        {
            del_sprite(_canvas);
        }

        void push(int32_t x, int32_t y)
        {
            if (_parent == nullptr) {
                _canvas.pushSprite(x, y);
            } else {
                _canvas.pushToSprite(_parent, x, y);
            }
        }

        void push_transparent(int32_t x, int32_t y)
        {
            if (_parent == nullptr) {
                _canvas.pushSprite(x, y, TFT_TRANSPARENT);
            } else {
                _canvas.pushToSprite(_parent, x, y, TFT_TRANSPARENT);
            }
        }

    private:
        TFT_eSprite* _parent = nullptr;

        static void create_sprite(TFT_eSprite& sprite, int16_t w, int16_t h, uint32_t bg_color)
        {
            sprite.createSprite(w, h);
            sprite.setColorDepth(16);
            sprite.setSwapBytes(true);
            sprite.fillSprite(bg_color);
        }

        static void del_sprite(TFT_eSprite& sprite)
        {
            sprite.deleteSprite();
        }
};