#pragma once

#include "UIElement.h"
#include "UVProgress.h"
#include "UIContainer.h"
#include "UILabel.h"
#include "UISoundAnalyzer.h"

template <size_t TChannels>
class UISoundEqualizer : public UIContainer 
{
public:    
    UISoundEqualizer(const UIRect& rect) : UIContainer(rect),
        bands({ rect.x + 30, rect.y + 25, rect.w - 50, rect.h - 120 - 23})
    {
        for (int i=0; i< (sizeof(level_lables) / sizeof(level_lables[0])); i++)
        {
            Add(level_lables[i]);
        }

         Add(bands);
    }

    UISoundAnalyzer<TChannels> bands;

private:
    UILabel level_lables[7] {
        {{ 10, 18,         20, 16 }, "  0"},
        {{ 5, 18 + 20 * 1, 20, 16 }, "-10"},
        {{ 5, 18 + 20 * 2, 20, 16 }, "-20"},
        {{ 5, 18 + 20 * 3, 20, 16 }, "-30"},
        {{ 5, 18 + 20 * 4, 20, 16 }, "-40"},
        {{ 5, 18 + 20 * 5, 20, 16 }, "-50"},
        {{ 5, 18 + 20 * 6, 20, 16 }, "-60"}
    };
};

class MainForm : public UIContainer
{
public:
  MainForm(const UIRect& rect) : 
    UIContainer(rect),

    volume({ 320 - 50, 0, 50, 20 }, "100%"),
    track({ 0, 0, 320, 20 }, "Test"),

    equalizer({ 0, 0, 320, 240 - 23 }),

    levelLeft({ 24, 181,           246, 15 }, 0, 32767, 32767 * 0.9, 15),
    levelRight({ 24, 181 + 15 + 3, 246, 15 }, 0, 32767, 32767 * 0.9, 20),

    header({ 0, 0, 320, 23 }),
    footer({ 0, 240-23, 320, 23 }),

    levelLeftText({ 0, 181, 20, 16 }, "L"),
    levelRightText({ 0, 181 + 13 + 3, 20, 16 }, "R")
  { 
    header.Add(track);
    header.Add(volume); 
    
    Add(header);

    Add(equalizer);

    Add(levelLeftText);
    Add(levelRightText);

    Add(levelLeft);
    Add(levelRight);

    for (int i=0; i<icons_count; i++)
    {
        footer.Add(icons[i]);
    }
    
    Add(footer);
  }

    UILabel volume;
    UILabel track;

    UISoundEqualizer<30> equalizer;

    UVProgressOf<int16_t> levelLeft;
    UVProgressOf<int16_t> levelRight;

    void setIcon(int index, bool state)
    {
        if (index<0 || index >= icons_count){
            return;
        }

        auto color = state ? icons_state_on[index] : icons_state_off[index];

        icons[index].setForecolor(color);
        icons[index].setBorderColor(color);
    }

private:
    UIContainer header;
    UIContainer footer;

    UILabel levelLeftText;
    UILabel levelRightText;

    UILabel icons[5] {
        {{ 0, 0, 50, 18 },                        "COAX", Color::Gray, Color::Gray, 2},
        {{ 50 + 2, 0, 42, 18 },                   "AUX",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2, 0, 42, 18 },          "Web",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2 + 42 + 2, 0, 50, 18 }, "A2DP", Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2 + 42 + 2 + 50 + 2, 0, 50, 18 }, "MUTE", Color::Gray, Color::Gray, 2}
    };

    const Color icons_state_on[5] {
        Color::Red,
        Color::Orange,
        Color::Orange,
        Color::LightBlue,
        Color::White
    };

    const Color icons_state_off[5] {
        Color::Gray,
        Color::Gray,
        Color::Gray,
        Color::Gray,
        Color::Gray
    };

    const int icons_count = sizeof(icons) / sizeof(icons[0]);
};

// class InternetRadioView : public UIContainer
// {
// public: 
//   InternetRadioView(const RadioStation* stations) 
//     : UIContainer({ 0, 0, 320, 240 - 23 })
//   {
//     UIList<RadioStation> stationsList({ 0, 0, 320, 240 - 23 });

//     // for (auto i=0; i < sizeof(Stations)/sizeof(Stations[0]); i++)
//     // {
//     //     stations.Add(Stations[i]);
//     // } 
//     Add(stationsList);
//   }

// private:
//     RadioStation* _stations;
// };