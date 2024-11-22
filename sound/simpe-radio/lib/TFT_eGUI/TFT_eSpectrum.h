#pragma once

#include <vector>

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"

#define TFT_SA_DARK_YELLOW tft_color24to16(0x23230F)
#define TFT_SA_YELLOW tft_color24to16(0xF3E835)

#define TFT_SA_DARK_GREEN tft_color24to16(0x090E07)
#define TFT_SA_GREEN tft_color24to16(0x64C749)

template <int TSize>
class TFT_eSpectrum : public TFT_eWidget
{
    public:
        TFT_eSpectrum(TFT_eSPI *tft, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
        : TFT_eWidget(tft, width, height, top, left)
        {
        }

        inline void init()
        {   
            create(get_band_w(), get_band_h(), background_color);
        }

        void update()
        {
            auto bar_w = get_band_w();
            auto bar_h = get_band_h();

            auto actual_top = top + band_segment_padding;
            auto half_segment_padding = band_segment_padding / 2;

            for (auto i = 0; i<bands_count; ++i) 
            {
                auto actual_left = left + half_segment_padding + i * (bar_w + band_segment_padding);

                auto y = map(constrain(_values[i], _min, _max), _min, _max, 0, bar_h);

                //canvas->fillRectHGradient(0, 0, bar_w, bar_h - y, TFT_DARK_GRAY, TFT_DARK_DARK_GRAY);
                _canvas.fillSprite(background_color);

                //_canvas->fillRect(0, 0, bar_w, bar_h - y, background_color);
                _canvas.fillRectHGradient(0, y, bar_w, bar_h - y, bar_color_gradient_from, bar_color_gradient_to);

                push(left + bar_w * i + band_segment_padding * i, top);
            }
        }

        //uint16_t foreground_color = TFT_DARK_DARK_GRAY;
        uint16_t background_color = TFT_DARK_DARK_GRAY;

        uint16_t bar_color_gradient_from = TFT_GREENYELLOW;
        uint16_t bar_color_gradient_to = TFT_GREEN;
        
        const int bands_count = TSize;

        uint8_t band_segments = 100;
        uint8_t band_segment_padding = 2;
        uint8_t band_segment_height = 2;

    private:
        float _values[TSize];
        float _min = 0;
        float _max = 255;

        inline int get_band_w() const {
            return width / bands_count - band_segment_padding;
        }

        inline int get_band_h() const {
            return height - band_segment_padding * 2;
        }
};

// template <int TSize>
// void gui_spectrum_init(TFT_eSpectrum<TSize>& spectrum) 
// {   
//     not_null(spectrum.canvas);

//     spectrum.canvas->setColorDepth(16);
//     //spectrum.canvas->createSprite(spectrum.width, spectrum.height);
//     spectrum.canvas->setSwapBytes(true);
//     // spectrum.canvas->fillSprite(spectrum.background_color);
//     // spectrum.canvas->pushSprite(spectrum.left, spectrum.top);
//     // spectrum.canvas->deleteSprite();

//     auto band_w = spectrum.width / spectrum.bands - spectrum.band_segment_padding;
//     auto band_h = spectrum.height - spectrum.band_segment_padding * 2;

//     spectrum.canvas->createSprite(band_w, band_h);
// }

// template <int TSize>
// void gui_spectrum_update(const TFT_eSpectrum<TSize>& spectrum) 
// {
//     not_null(spectrum.canvas);

//     auto bar_w = spectrum.width / spectrum.bands - spectrum.band_segment_padding;
//     auto bar_h = spectrum.height - spectrum.band_segment_padding * 2;

//     auto top = spectrum.top + spectrum.band_segment_padding;
//     auto half_segment_padding = spectrum.band_segment_padding / 2;

//     for (auto i = 0; i<spectrum.bands; ++i) 
//     {
//         auto left = spectrum.left + half_segment_padding + i * (bar_w + spectrum.band_segment_padding);   
//         auto y = map(spectrum.values[i], spectrum.min, spectrum.max, 0, bar_h);

//         //spectrum.canvas->fillRectHGradient(0, 0, bar_w, bar_h - y, TFT_DARK_GRAY, TFT_DARK_DARK_GRAY);
//         //spectrum.canvas->fillSprite(spectrum.background_color);
//         spectrum.canvas->fillRect(0, 0, bar_w, bar_h - y, spectrum.background_color);
//         spectrum.canvas->fillRectHGradient(0, y, bar_w, bar_h - y, TFT_GREENYELLOW, TFT_GREEN);

//         spectrum.canvas->pushSprite(left, top);
//     }
// }

// template <int TSize>
// void gui_spectrum_begin(const TFT_eSpectrum<TSize>& spectrum) 
// {
//     gui_spectrum_update(spectrum);
// }