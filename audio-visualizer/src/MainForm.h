#pragma once

#include "UIElement.h"
#include "UIContainer.h"
#include "UILabel.h"
#include "UISoundAnalyzer.h"

class MainForm : public UIContainer
{
public:
  MainForm(const UIRect& rect) : 
    UIContainer(rect),

    volume({ 320 - 50, 0, 50, 20 }, "100%"),
    track({ 0, 0, 320, 20 }, "Test"),

    levelLeft({ 24, 181,           246, 15 }, 0, 32767, 32767 * 0.9, 100),
    levelRight({ 24, 181 + 15 + 3, 246, 15 }, 0, 32767, 32767 * 0.9, 100),

    header({ 0, 0, 320, 23 }),
    footer({ 0, 240-23, 320, 23 }),

    levelLeftText({ 0, 181, 20, 16 }, "L"),
    levelRightText({ 0, 181 + 13 + 3, 20, 16 }, "R")
  { 
    header.Add(track);
    header.Add(volume); 
    
    Add(header);

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

    const int icons_count = 5;// sizeof(icons) / sizeof(icons[0]);
};

// class SoundMetersView : public UIContainer
// {
// public:
//   SoundMetersView() : 
//     UIContainer({ 0, 0, 320, 240 - 23 }), 
//     _analyzer({ 30, 25, 270, 120 })
//   {
//     memset(peak, 0, BANDS_COUNT);

//     Add(_analyzer);
//   }

//   void updateBand(int band, int amplitude)
//   {
//     if (amplitude > AMPLITUDE_MAX) 
//     {
//       amplitude = AMPLITUDE_MAX;
//     }

//     _analyzer.Update(band, amplitude);

//     if (amplitude > peak[band]) 
//     {
//       peak[band] = amplitude;
//     }
//   }

// private:
//   const static int BANDS_COUNT = 60;
//   const static int AMPLITUDE_MAX = 255;

//   UISoundAnalyzer<BANDS_COUNT> _analyzer;
//   unsigned char peak[BANDS_COUNT];
// };

// std::ostream& operator<<(std::ostream& os, const RadioStation& station)
// {
//     os << station.Name;
//     return os;
// }

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