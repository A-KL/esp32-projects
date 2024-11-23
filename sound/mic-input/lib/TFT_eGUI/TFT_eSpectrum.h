#pragma once

#include <vector>

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"

#define TFT_SA_DARK_YELLOW tft_color24to16(0x23230F)
#define TFT_SA_YELLOW tft_color24to16(0xF3E835)

#define TFT_SA_DARK_GREEN tft_color24to16(0x090E07)
#define TFT_SA_GREEN tft_color24to16(0x64C749)

template <int TSize>
struct TFT_eSpectrum
{
    int left = 0;
    int top = 0;

    int width = 150;
    int height = 100;

    int foreground_color = TFT_DARK_DARK_GRAY;
    int background_color = TFT_DARK_DARK_GRAY;

    TFT_eSprite* canvas = NULL;
    const int bands;
    const int band_segments;
    const int band_segment_padding = 2;
    const int band_segment_height = 2;

    float values[TSize];
    float min = 0;
    float max = 255;

    TFT_eSpectrum(TFT_eSprite& canvas, const int band_resolution) 
        : canvas(&canvas), bands(TSize), band_segments(band_resolution)
    { }    
};

template <int TSize>
void gui_spectrum_init(TFT_eSpectrum<TSize>& spectrum) 
{   
    not_null(spectrum.canvas);

    spectrum.canvas->setColorDepth(16);
    //spectrum.canvas->createSprite(spectrum.width, spectrum.height);
    spectrum.canvas->setSwapBytes(true);
    // spectrum.canvas->fillSprite(spectrum.background_color);
    // spectrum.canvas->pushSprite(spectrum.left, spectrum.top);
    // spectrum.canvas->deleteSprite();

    int bar_w = spectrum.width / spectrum.bands - spectrum.band_segment_padding;
    int bar_h = spectrum.height - spectrum.band_segment_padding * 2;

    spectrum.canvas->createSprite(bar_w, bar_h);
}

template <int TSize>
void gui_spectrum_update(const TFT_eSpectrum<TSize>& spectrum) 
{
    not_null(spectrum.canvas);

    int bar_w = spectrum.width / spectrum.bands - spectrum.band_segment_padding;
    int bar_h = spectrum.height - spectrum.band_segment_padding * 2;

    auto top = spectrum.top + spectrum.band_segment_padding;
    auto half_segment_padding = spectrum.band_segment_padding / 2;

    for (auto i = 0; i<spectrum.bands; ++i) 
    {
        auto left = spectrum.left + half_segment_padding + i * (bar_w + spectrum.band_segment_padding);   
        auto y = map(spectrum.values[i], spectrum.min, spectrum.max, 0, bar_h);

        //spectrum.canvas->fillRectHGradient(0, 0, bar_w, bar_h - y, TFT_DARK_GRAY, TFT_DARK_DARK_GRAY);
        //spectrum.canvas->fillSprite(spectrum.background_color);
        spectrum.canvas->fillRect(0, 0, bar_w, bar_h - y, spectrum.background_color);
        spectrum.canvas->fillRectHGradient(0, y, bar_w, bar_h - y, TFT_GREENYELLOW, TFT_GREEN);

        spectrum.canvas->pushSprite(left, top);
    }
}

template <int TSize>
void gui_spectrum_begin(const TFT_eSpectrum<TSize>& spectrum) 
{
    gui_spectrum_update(spectrum);
}