#pragma once

#include "UIElement.h"
#include "UIContainer.h"
#include "UILabel.h"
#include "UISoundAnalyzer.h"

struct RadioStation 
{
    const char* Name;
    const char* Url;

    friend std::ostream& operator<<(std::ostream& os, const RadioStation& station);
};

// class MainForm : public UIContainer
// {
// public:
//   MainForm() : 
//     UIContainer({ 0, 0, 320, 240 }),
//     _header(UIRect({ 0, 0, 320, 23 })),
//     _footer(UIRect({ 0, 240-23, 320, 23 }), Color({ 56, 56, 56, 0 })),
//     _input({ 0, 0, 320, 25 }, _inputSources[0], NULL, 16),
//     _volume({ 0, 240-20, 40, 23 }, "VOL:0", NULL, 16)
//   { 
//     _header.Add(_input); 
//     _footer.Add(_volume);
    
//     Add(_header);
//     Add(_footer);
//   }

//   void setVolume(int value)
//   {
//     if (value == 0){
//       _volume.SetText("MUTE");
//     }
//     else
//     {
//       char buffer[5];
//       snprintf(buffer, sizeof(buffer), "VOL:%d", value);
//       _volume.SetText(buffer);
//     }
//   }

//   void setInput(int value = 0)
//   {   
//     _input.SetText(_inputSources[value]);
//   }

// private:
//     UIContainer _header;
//     UIContainer _footer;
//     UILabel _input;
//     UILabel _volume;

//     const char* _inputSources[4] =
//     {
//       "Off", "S/PDIF", "Analog In", "Radio"
//     };
// };

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