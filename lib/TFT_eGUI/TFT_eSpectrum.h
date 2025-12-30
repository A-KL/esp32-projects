#pragma once

// #include <vector>
#include "TFT_eSPI_Ex.h"
#include "TFT_eWidget.h"

#define TFT_SA_DARK_YELLOW tft_color24to16(0x23230F)
#define TFT_SA_YELLOW tft_color24to16(0xF3E835)

#define TFT_SA_DARK_GREEN tft_color24to16(0x090E07)
#define TFT_SA_GREEN tft_color24to16(0x64C749)

template <int TSize>
class TFT_eSpectrum : public TFT_eWidget
{
    public:
        TFT_eSpectrum(TFT_eSPI *tft, int16_t width, int16_t height, int16_t left = 0, int16_t top = 0) 
        : TFT_eWidget(tft, width, height, left, top)
        { }

        inline int size() const {
            return TSize;
        }

        void init() {   
            for (auto i=0; i<size(); i++) {
                _values[i] = _min;
                _values_new[i] = _min;
            }
        }

        void begin()
        {
            create(width, height, background_color);
            push(left, top);
            del();

            create(get_band_w(), get_band_h(), background_color);

            update(true);
        }

        inline void update()
        {
            update(false);
        }

        TFT_eSpectrum& set_value(uint8_t index, float value)
        {
            _values_new[index] = constrain(value, _min, _max);
            return *this;
        }

        uint8_t bar_padding = 2;
        uint16_t background_color = TFT_DARK_DARK_GRAY;
        uint16_t bar_background_color = TFT_DARKGREEN;
        uint16_t bar_foreground_color = TFT_GREEN;

        const TFT_eColorBrush* empty_bar_style = NULL;
        const TFT_eColorBrush* full_bar_style = NULL;

    private:
        float _values[TSize];
        float _values_new[TSize];
        float _min = 0;
        float _max = 255;

        inline int get_band_w() const {
            return round((float)width/ size()) - bar_padding;
        }

        inline int get_band_h() const {
            return height;
        }

        void update(bool force)
        {
            auto bar_w = get_band_w();
            auto bar_h = get_band_h();

            auto actual_top = top + bar_padding;
            auto half_segment_padding = bar_padding / 2;

            for (auto i = 0; i<size(); ++i) 
            {
                if (_values_new[i] == _values[i] && !force) 
                {
                    continue;
                }

                _values[i] = _values_new[i];

                //auto actual_left = left + half_segment_padding + i * (bar_w + band_segment_padding);

                auto y = fmap(_values[i], _min, _max, bar_h, 0);

                //canvas->fillRectHGradient(0, 0, bar_w, bar_h - y, TFT_DARK_GRAY, TFT_DARK_DARK_GRAY);
                //_canvas.fillSprite(background_color);

                if (empty_bar_style) {
                    empty_bar_style->fillRect(&_canvas, 0, 0, bar_w, y);
                } 
                else {
                    _canvas.fillRect(0, 0, bar_w, y, bar_background_color);  
                }

                if (full_bar_style) {
                    full_bar_style->fillRect(&_canvas, 0, y, bar_w, bar_h - y);
                }
                else {
                    _canvas.fillRect(0, y, bar_w, bar_h - y, bar_foreground_color);
                }

                push(left + bar_w * i + bar_padding * i, top);
            }
        }
};