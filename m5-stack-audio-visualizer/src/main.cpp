#include <limits.h>
#include <Arduino.h>
#include <arduinoFFT.h>

#include "Network.h"

#include "Color.h"
#include "Canvas.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
#endif

#include "UI.h"

// ---------------------------------------------------
#define ADC_MIC          34
#define ADC_CHANNEL_LEFT ADC_MIC
#define ADC_CHANNEL_RIGHT 35

#define SAMPLES 512              // Must be a power of 2
#define BANDS_COUNT 30 
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE_MAX 255

unsigned int sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
unsigned long newTime, oldTime, microseconds;

unsigned char peak[BANDS_COUNT];

double vReal_l[SAMPLES];
double vReal_r[SAMPLES];

double vImag_l[SAMPLES];
double vImag_r[SAMPLES];

// ---------------------------------------------------
arduinoFFT fft;

//M5StackCanvas canvas;

#ifdef ESP_WROVER
  TFTCanvas canvas;
#endif

// ---------------------------------------------------

void displayBand(UISoundAnalyzer<BANDS_COUNT>& analyzer, int band, int amplitude)
{
  if (amplitude > AMPLITUDE_MAX) 
  {
    amplitude = AMPLITUDE_MAX;
  }

  analyzer.Update(band, amplitude);

  if (amplitude > peak[band]) 
  {
    peak[band] = amplitude;
  }
}

void setup() 
{
  Serial.begin(115200);

  canvas.Init(Color(0, 0, 0));

  memset(peak, 0, BANDS_COUNT);

  StartWifi();


  // UI
  UIContainer panel({ 0, 0, 320, 240 });

  // Title
  const char* font = NULL;
  UILabel label({ 0, 0, 320, 25 }, "S/PDIF", font, 16);

  // Main
	auto start = 18;
	UILabel label_0({ 8, start, 18, 16 }, "0", font, 16);
	UILabel label_10({ 5, start += 19, 20, 16 }, "-10", font, 16);
	UILabel label_20({ 5, start += 20, 20, 16 }, "-20", font, 16);
	UILabel label_30({ 5, start += 19, 20, 16 }, "-30", font, 16);
	UILabel label_40({ 5, start += 20, 20, 16 }, "-40", font, 16);
	UILabel label_50({ 5, start += 19, 20, 16 }, "-50", font, 16);
	UILabel label_60({ 5, start += 20, 20, 16 }, "-60", font, 16);

	UISoundAnalyzer<BANDS_COUNT> analyzer({ 30, 25, 270, 120 });

  // Levels
  UILabel level_left_label({ 0, 181, 20, 16 }, "L", font, 16);
  UILabel level_right_label({ 0, 181 + 13 + 3, 20, 16 }, "R", font, 16);

  UVProgress<uint8_t> level_left({ 24, 181,           246, 15 }, 0, 4095, 4095 * 0.9, (uint8_t)0);
  UVProgress<uint8_t> level_right({ 24, 181 + 15 + 3, 246, 15 }, 0, 4095, 4095 * 0.9, (uint8_t)0);

  level_left.Clear(canvas);
  level_right.Clear(canvas);

  // Footer
  UIContainer footer({ 0, 240-23, 320, 23 });
  footer.Background = { 56, 56, 56, 0 };
  footer.Clear(canvas);

  // Build UI
	panel.Add(label);
	panel.Add(label_0);
	panel.Add(label_10);
	panel.Add(label_20);
	panel.Add(label_30);
	panel.Add(label_40);
	panel.Add(label_50);
	panel.Add(label_60);
  panel.Add(analyzer);
  panel.Add(level_left);
  panel.Add(level_right);
  panel.Add(level_left_label);
  panel.Add(level_right_label);
  panel.Add(footer);

  while (true)
	{
    unsigned long sum_l = 0;
    unsigned long sum_r = 0;

    for (int i = 0; i < SAMPLES; i++) {

      newTime = micros()-oldTime;
      oldTime = newTime;

      vReal_l[i] = 1110; //analogRead(ADC_CHANNEL_LEFT); // A conversion takes about 1uS on an ESP32
      vReal_r[i] = 1230;// analogRead(ADC_CHANNEL_RIGHT);

      vImag_l[i] = 0;
      vImag_r[i] = 0;

      sum_l += vReal_l[i];
      sum_r += vReal_r[i];

      while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
    }

    fft.Windowing(vReal_l, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    fft.Compute(vReal_l, vImag_l, SAMPLES, FFT_FORWARD);
    fft.ComplexToMagnitude(vReal_l, vImag_l, SAMPLES);

   // double peak = fft.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.

    for (int band_index = 0, bin = 2; band_index < BANDS_COUNT; band_index++, bin+=4)
    {
      if (vReal_l[bin] < 400) { // Add a crude noise filter, 10 x amplitude or more
        continue;
      }
        displayBand(analyzer, band_index, (int)vReal_l[bin] / AMPLITUDE_MAX); 
    }

    // for (int band_index = 17, bin = 100; band_index < BANDS_COUNT; band_index++, bin+=2)
    // {      
    //   if (vReal_l[bin] < 400) { // Add a crude noise filter, 10 x amplitude or more
    //     continue;
    //   }
    //     displayBand(analyzer, band_index, (int)vReal_l[bin] / AMPLITUDE_MAX); 
    // }

    // for (int i = 2, band_index = 0;  i < (SAMPLES/2); i+=2)
    // { 
    //   if (vReal_l[i] < 400) { // Add a crude noise filter, 10 x amplitude or more
    //     continue;
    //   }

    //   if (band_index>=30) {
    //     break;
    //   }

    //   displayBand(analyzer, band_index++, (int)vReal_l[i] / AMPLITUDE_MAX); 

    //     // if (i<=2           ) displayBand(analyzer, band_index++, (int)vReal_l[i] / AMPLITUDE_MAX);           
    //     // if (i >3   && i<=5 )   displayBand(analyzer, 1, (int)vReal[i]/amplitude); // 80Hz
    //     // if (i >5   && i<=7 )   displayBand(analyzer, 2, (int)vReal[i]/amplitude); // 500Hz
    //     // if (i >7   && i<=15 )  displayBand(analyzer, 3, (int)vReal[i]/amplitude); // 1000Hz
    //     // if (i >15  && i<=30 )  displayBand(analyzer, 4, (int)vReal[i]/amplitude); // 2000Hz
    //     // if (i >30  && i<=53 )  displayBand(analyzer, 5, (int)vReal[i]/amplitude); // 4000Hz
    //     // if (i >53  && i<=200 ) displayBand(analyzer, 6, (int)vReal[i]/amplitude); // 8000Hz
    //     // if (i >200           ) displayBand(analyzer, 7, (int)vReal[i]/amplitude); // 16000Hz  

    //   // for (byte band = 0; band <= 6; band++) 
    //   //     display.drawHorizontalLine(18*band,64-peak[band],14);
    // }

    // if (millis()%4 == 0) 
    // {
    //   for (byte band = 0; band <= 6; band++)
    //   {
    //   if (peak[band] > 0) 
    //     peak[band] -= 1;
    //   }
    // } // Decay the peak


    level_left.SetFinal(abs((sum_l / (float)SAMPLES) - 2000));
   // level_right.SetFinal(abs((sum_r / (float)SAMPLES) - 1980));

    while (!level_left.IsValid() ) //|| !level_right.IsValid()
    {
      level_left.Draw(canvas);
      level_right.Draw(canvas);
      delay(2);
    }

    panel.Draw(canvas);
	}
}

void loop() {
}