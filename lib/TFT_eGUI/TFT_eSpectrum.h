#pragma once

#include <vector>
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

        inline void init()
        {   
            create(get_band_w(), get_band_h(), background_color);

            for (auto i=0; i<bands_count; i++) {
                _values[i] = _min;
                _values_new[i] = _min;
            }
        }

        void begin()
        {
            update(true);
        }

        void update()
        {
            update(false);
        }

        inline TFT_eSpectrum& set_value(uint8_t index, float value)
        {
            _values_new[index] = constrain(value, _min, _max);
            return *this;
        }

        //uint16_t foreground_color = TFT_DARK_DARK_GRAY;
        uint16_t background_color = TFT_DARK_DARK_GRAY;

        uint16_t bar_color_gradient_from = TFT_GREENYELLOW;
        uint16_t bar_color_gradient_to = TFT_GREEN;
        
        const int bands_count = TSize;

        uint8_t band_segments = 100;
        uint8_t band_segment_padding = 2;
        //uint8_t band_segment_height = 2;

    private:
        float _values[TSize];
        float _values_new[TSize];
        float _min = 0;
        float _max = 255;

        inline int get_band_w() const {
            return width / bands_count - band_segment_padding;
        }

        inline int get_band_h() const {
            return height - band_segment_padding * 2;
        }

        void update(bool force)
        {
            auto bar_w = get_band_w();
            auto bar_h = get_band_h();

            auto actual_top = top + band_segment_padding;
            auto half_segment_padding = band_segment_padding / 2;

            for (auto i = 0; i<bands_count; ++i) 
            {
                if (_values_new[i] == _values[i] && !force) 
                {
                    continue;
                }

                _values[i] = _values_new[i];

                auto actual_left = left + half_segment_padding + i * (bar_w + band_segment_padding);

                auto y = map(_values[i], _min, _max, bar_h, 0);

                //canvas->fillRectHGradient(0, 0, bar_w, bar_h - y, TFT_DARK_GRAY, TFT_DARK_DARK_GRAY);
                _canvas.fillSprite(background_color);

                //_canvas->fillRect(0, 0, bar_w, bar_h - y, background_color);
                _canvas.fillGradientRect(0, y, bar_w, bar_h - y, bar_color_gradient_from, bar_color_gradient_to, lgfx::HLINEAR);
                //fillRectHGradient

                push(left + bar_w * i + band_segment_padding * i, top);
            }
        }        
};