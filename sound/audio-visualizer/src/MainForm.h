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
        bands({ 30, 10, rect.w - 50, rect.h - 120 - 23})
    {
        for (int i=0; i< (sizeof(level_lables) / sizeof(level_lables[0])); i++)
        {
            Add(level_lables[i]);
        }
        
        for (int i=0; i< (sizeof(freq_lables) / sizeof(freq_lables[0])); i++)
        {
            Add(freq_lables[i]);
        }
         Add(bands);
    }

    UISoundAnalyzer<TChannels> bands;

private:
    UILabel level_lables[7] {
        {{ 17, 0,     10, 16 }, "0"},
        {{ 5, 20 * 1, 20, 16 }, "-10"},
        {{ 5, 20 * 2, 20, 16 }, "-20"},
        {{ 5, 20 * 3, 20, 16 }, "-30"},
        {{ 5, 20 * 4, 20, 16 }, "-40"},
        {{ 5, 20 * 5, 20, 16 }, "-50"},
        {{ 5, 20 * 6, 20, 16 }, "-60"}
    };

    UILabel freq_lables[5] {
        {{ 25,          20 * 7 + 3,  20, 16 }, "25",  1},
        {{ 25 + 20 * 1, 20 * 6 + 10, 20, 16 }, "40",  1},
        {{ 25 + 20 * 2, 20 * 7 + 3,  20, 16 }, "63",  1},
        {{ 25 + 20 * 3, 20 * 6 + 10, 25, 16 }, "100", 1},
        {{ 25 + 20 * 4, 20 * 7 + 3,  25, 16 }, "157", 1},

    };
};

class MainForm : public UIContainer
{
public:
  MainForm(const UIRect& rect) : 
        UIContainer(rect),

        volume({ 320 - 40, 0, 40, 20 }, "-20db"),
        track({ 0, 0, 320 - 40, 20 }, "[Title]"),

        equalizer({ 0, 20, 320, 240 - 23 - 20 }),

        levelLeft({ 24, 181,           246, 15 }, 0, USHRT_MAX, USHRT_MAX * 0.95, 0),
        levelRight({ 24, 181 + 15 + 3, 246, 15 }, 0, USHRT_MAX, USHRT_MAX * 0.95, 0),

        header({ 0, 0, 320, 20 }),
        footer({ 0, 240-18, 320, 18 }),

        leftTextL({ 0, 181, 20, 15 }, "L"),
        rightTextL({ 0, 181 + 15 + 3, 20, 15 }, "R"),

        leftTextValue({ 24 + 246 + 10, 181, 25, 15 }, "1.0"),
        rightTextValue({ 24 + 246 + 10, 181 + 15 + 3, 25, 15 }, "1.0")
    { 
        header.Add(track);
        header.Add(volume); 
        
        Add(header);

        Add(equalizer);

        Add(leftTextL);
        Add(rightTextL);

        Add(levelLeft);
        Add(levelRight);

        Add(leftTextValue);
        Add(rightTextValue);

        for (int i=0; i<icons_count; i++)
        {
            footer.Add(icons[i]);
        }
        
        Add(footer);
    }

    UILabel volume;
    UILabel track;

    UISoundEqualizer<30> equalizer;

    UVProgressOf<ushort> levelLeft;
    UVProgressOf<ushort> levelRight;

    void setIcon(int index, bool state)
    {
        if (index < 0 || index >= icons_count){
            return;
        }

        auto color = state ? icons_state_on[index] : icons_state_off[index];

        icons[index].setForecolor(color);
        icons[index].setBorderColor(color);
    }

//protected:
	// void Draw(Canvas<Color>& canvas)
	// {
    //     if (!levelLeft.IsValid()) {
    //         //leftTextValue.setInt(levelLeft.value());
    //     }

    //     if (!levelRight.IsValid()) {
    //         //rightTextValue.setInt(levelRight.value());
    //     }        

    //     UIContainer::Draw(canvas);
    // }

private:
    UIContainer header;
    UIContainer footer;

    UILabel leftTextL;
    UILabel rightTextL;

    UILabel leftTextValue;
    UILabel rightTextValue;

    UILabel icons[6] {
        {{ 0,                                          0, 50, 18 }, "COAX", Color::Gray, Color::Gray, 2},
        {{ 50 + 2,                                     0, 42, 18 }, "AUX",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2,                            0, 42, 18 }, "Web",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2 + 42 + 2,                   0, 50, 18 }, "LINE", Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2 + 42 + 2 + 50 + 2,          0, 50, 18 }, "A2DP", Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 42 + 2 + 42 + 2 + 50 + 2 + 50 + 2, 0, 50, 18 }, "MUTE", Color::Gray, Color::Gray, 2}
    };

    const Color icons_state_on[6] {
        Color::Orange,
        Color::Green,

        Color::Purpule,
        Color::Purpule,
        Color::LightBlue,
        
        Color::Red,
    };

    const Color icons_state_off[6] {
        Color::Gray,
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