#pragma once

#include "UIRect.h"
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
        bands({ 30, 7, rect.w - 50, rect.h - 75})
    {
        for (auto& level_label : level_labels) {
            Add(level_label);
        }
        for (auto& freq_label : freq_labels) {
            Add(freq_label);
        }
        Add(bands);
    }

    UISoundAnalyzer<TChannels> bands;

private:
    UILabel level_labels[7] {
        {{ 17, 0,     10, 16 }, "0"},
        {{ 5, 20 * 1, 20, 16 }, "-10"},
        {{ 5, 20 * 2, 20, 16 }, "-20"},
        {{ 5, 20 * 3, 20, 16 }, "-30"},
        {{ 5, 20 * 4, 20, 16 }, "-40"},
        {{ 5, 20 * 5, 20, 16 }, "-50"},
        {{ 5, 20 * 6, 20, 16 }, "-60"}
    };

    UILabel freq_labels[15] {
        {{ 25,           20 * 7 + 3,  20, 16 }, "25",  Color::Gray, Color::Black },
        {{ 25 + 20 * 1,  20 * 6 + 10, 20, 16 }, "40",  Color::Gray, Color::Black },
        {{ 25 + 20 * 2,  20 * 7 + 3,  20, 16 }, "63",  Color::Gray, Color::Black },
        {{ 25 + 20 * 3,  20 * 6 + 10, 25, 16 }, "100", Color::Gray, Color::Black },
        {{ 25 + 20 * 4,  20 * 7 + 3,  25, 16 }, "157", Color::Gray, Color::Black },
        {{ 25 + 20 * 5,  20 * 6 + 10, 25, 16 }, "250", Color::Gray, Color::Black },
        {{ 25 + 20 * 6,  20 * 7 + 3,  25, 16 }, "400", Color::Gray, Color::Black },
        {{ 25 + 20 * 7,  20 * 6 + 10, 25, 16 }, "630", Color::Gray, Color::Black },
        {{ 25 + 20 * 8,  20 * 7 + 3,  25, 16 }, "1k",  Color::Gray, Color::Black },
        {{ 25 + 20 * 9,  20 * 6 + 10, 25, 16 }, "1k6", Color::Gray, Color::Black },
        {{ 25 + 20 * 10, 20 * 7 + 3,  25, 16 }, "2k5", Color::Gray, Color::Black },
        {{ 25 + 20 * 11, 20 * 6 + 10, 25, 16 }, "4k",  Color::Gray, Color::Black },
        {{ 25 + 20 * 12, 20 * 7 + 3,  25, 16 }, "6k3", Color::Gray, Color::Black },
        {{ 25 + 20 * 13, 20 * 6 + 10, 25, 16 }, "10k", Color::Gray, Color::Black },
        {{ 25 + 20 * 14, 20 * 7 + 3,  25, 16 }, "16k", Color::Gray, Color::Black }
    };
};

class MainForm : public UIContainer
{
public:
  MainForm(const UIRect& rect) : UIContainer(rect),

        volume({ 320 - 50, 0, 50, 20 }, "-20db"),
        track({ 0, 0, 320 - 50, 20 }, "[Title]"),

        equalizer({ 0, 20, 320, 240 - 23 - 20 }),

        // levelLeft({ 24, 181,           246, 15 }, 0, USHRT_MAX/4, USHRT_MAX/4 * 0.95, 0),
        // levelRight({ 24, 181 + 15 + 3, 246, 15 }, 0, USHRT_MAX/4, USHRT_MAX/4 * 0.95, 0),

        levelLeft({ 24, 181,           246, 15 }, 0, USHRT_MAX, USHRT_MAX * 0.95, 0),
        levelRight({ 24, 181 + 15 + 3, 246, 15 }, 0, USHRT_MAX, USHRT_MAX * 0.95, 0),

        header({ 0, 0, 320, 20 }),
        footer({ 10, 240-18, 320, 18 }),

        leftTextL({ 0, 181, 20, 15 }, "L"),
        rightTextL({ 0, 181 + 15 + 3, 20, 15 }, "R"),

        leftTextValue({ 24 + 246 + 10, 181, 25, 15 }, "1.0"),
        rightTextValue({ 24 + 246 + 10, 181 + 15 + 3, 25, 15 }, "1.0"),

        messageBox({40, 40, 250, 90}, Color(39, 46, 26)),
        messageBoxTitle({10, 10, 230, 20}, "WARNING", Color(120, 130, 100) /* Gray */, {0,0,0,0}, Color(39, 46, 26)),
        messageBoxText({10, 30, 230, 20}, "The radio station is selected", Color(243, 243, 238) /* White */, Color::Black, Color(39, 46, 26), 0, 0, 10)
    { 
        header.Add(track);
        header.Add(volume);

        messageBox.SetBorderColor(Color(151, 171, 92));
        messageBox.SetBorderSize(1);
        messageBox.SetVisible(false);

        messageBox.Add(messageBoxTitle);
        messageBox.Add(messageBoxText);
        Add(messageBox);
        
        Add(header);
        Add(equalizer);

        Add(leftTextL);
        Add(rightTextL);

        Add(levelLeft);
        Add(levelRight);

        Add(leftTextValue);
        Add(rightTextValue);

        for (auto& icon : icons) {
            footer.Add(icon);
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

        icons[index].SetForeColor(color);
        icons[index].SetBorderColor(color);
    }

    void showMessage(const char* message) 
    {
        equalizer.SetVisible(false);
        // messageBoxText.SetText(message);
        messageBox.SetVisible(true);
    }

    void hideMessage() 
    {
        messageBox.SetVisible(false);
        equalizer.SetVisible(true);
    }

private:
    UIContainer header;
    UIContainer footer;

    UILabel leftTextL;
    UILabel rightTextL;

    UILabel leftTextValue;
    UILabel rightTextValue;

    UIContainer messageBox;
    UILabel messageBoxTitle;
    UILabel messageBoxText;

    UILabel icons[6] {
        {{ 00,                                         0, 50, 18 }, "COAX", Color::Gray, Color::Gray, 2},
        {{ 50 + 2,                                     0, 50, 18 }, "AUX",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 50 + 2,                            0, 42, 18 }, "RDS",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 50 + 2 + 42 + 2,                   0, 50, 18 }, "LINE", Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 50 + 2 + 42 + 2 + 50 + 2,          0, 50, 18 }, "USB",  Color::Gray, Color::Gray, 2},
        {{ 50 + 2 + 50 + 2 + 42 + 2 + 50 + 2 + 50 + 2, 0, 50, 18 }, "MUTE", Color::Gray, Color::Gray, 2}
    };

    const Color icons_state_on[6] {
        Color::Orange,
        Color::Green,

        Color::Purple,
        Color::LightBlue,
        Color::Orange,
        
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