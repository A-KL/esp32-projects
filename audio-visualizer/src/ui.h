#pragma once

#include "UIElement.h"
#include "UVProgress.h"
#include "UISoundAnalyzer.h"
#include "UILabel.h"
#include "UIContainer.h"
#include "UIList.h"
#include "MainForm.h"

// ---------------------------------------------------
#define SAMPLES 512
#define BANDS_COUNT 30

unsigned char peak[BANDS_COUNT];

double vReal_l[SAMPLES];
double vReal_r[SAMPLES];

double vImag_l[SAMPLES];
double vImag_r[SAMPLES];

unsigned int sampling_period_us = round(1000000 * (1.0 / 44100));
unsigned long newTime, oldTime, microseconds;
// ---------------------------------------------------
static arduinoFFT fft;
static TaskHandle_t analyzerHandle;
static xQueueHandle audioFrameQueue = xQueueCreate(SAMPLES, sizeof(AudioFrame));
//static InternetRadio radio;

UILabel label_track({ 0, 0, 320, 20 }, "");
// ---------------------------------------------------

void onAudioFrameCallback(const AudioFrame& frame)
{
  xQueueSend(audioFrameQueue, &frame, 0);
}

void onStreamChanged(const char *type, const char *value)
{
  if (strcmp(type, "StreamTitle") == 0) { 
    label_track.setText(value);
    Serial.println(value);
  }
}

// void setupRadio()
// {
//   radio.Play(Stations[stationIndex].Url);
//   radio.SampleCallback(onAudioFrameCallback);
//   radio.StreamChanged = onStreamChanged;
// }

// void loopRadio()
// {
//     radio.Loop();
// }

// ---------------------------------------------------

void main_analyzer(void * args)
{
    //memset(peak, 0, BANDS_COUNT);

    auto canvas = (TCanvas*)args;

    canvas->SetFont(NULL, 1);

    // Root
    UIContainer panel({ 0, 0, 320, 240});

    // Analyzer
    UIContainer analyzer_panel({ 0, 0, 320, 240 - 23 });

    auto start = 18;
    UILabel label_0({ 10, start,       20, 16 }, "  0");
    UILabel label_10({ 5, start += 20, 20, 16 }, "-10");
    UILabel label_20({ 5, start += 20, 20, 16 }, "-20");
    UILabel label_30({ 5, start += 20, 20, 16 }, "-30");
    UILabel label_40({ 5, start += 20, 20, 16 }, "-40");
    UILabel label_50({ 5, start += 20, 20, 16 }, "-50");
    UILabel label_60({ 5, start += 20, 20, 16 }, "-60");

	  UISoundAnalyzer<BANDS_COUNT> analyzer({ 30, 25, 270, 120 });

    UILabel level_left_label({ 0, 181, 20, 16 }, "L");
    UILabel level_right_label({ 0, 181 + 13 + 3, 20, 16 }, "R");

    UVProgressOf<int16_t> level_left({ 24, 181,           246, 15 }, 0, 32767, 32767 * 0.9, 0);
    UVProgressOf<int16_t> level_right({ 24, 181 + 15 + 3, 246, 15 }, 0, 32767, 32767 * 0.9, 0);

    analyzer_panel.Add(label_track);
    analyzer_panel.Add(label_0);
    analyzer_panel.Add(label_10);
    analyzer_panel.Add(label_20);
    analyzer_panel.Add(label_30);
    analyzer_panel.Add(label_40);
    analyzer_panel.Add(label_50);
    analyzer_panel.Add(label_60);
    analyzer_panel.Add(analyzer);
    analyzer_panel.Add(level_left);
    analyzer_panel.Add(level_right);
    analyzer_panel.Add(level_left_label);
    analyzer_panel.Add(level_right_label);

    // Radio UI
    UIList<RadioStation> stations({ 0, 0, 320, 240 - 23 });

    for (auto i=0; i < sizeof(Stations)/sizeof(Stations[0]); i++)
    {
        stations.Add(Stations[0]);
    }

    //panel.Add(stations);

    // Footer
    UIContainer footer({ 0, 240-18, 320, 18 });

    UILabel label_vol({ 0, 0, 30, 23 }, "VOL:");

    UILabel label_out_spdif({ 0, 0, 50, 18 }, "COAX", Color::Red, 2);
	  label_out_spdif.setForecolor(Color::Red);

    UILabel label_out_aux({ 50 + 2, 0, 42, 18 }, "AUX", Color::Gray, 2);
	  label_out_aux.setForecolor(Color::Gray);

    UILabel label_input_web({ 50 + 2 + 42 + 2, 0, 42, 18 }, "Web", Color::Orange, 2);
	  label_input_web.setForecolor(Color::Orange);

    UILabel label_input_bt({ 50 + 2 + 42 + 2 + 42 + 2, 0, 50, 18 }, "A2DP", Color::Gray, 2);
	  label_input_bt.setForecolor(Color::Gray);

    UILabel label_input_mute({ 50 + 2 + 42 + 2 + 42 + 2 + 50 + 2, 0, 50, 18 }, "MUTE", Color::Gray, 2);
	  label_input_mute.setForecolor(Color::Gray);

    // UILabel label_input_aux({ 52, 0, 42, 18 }, "AUX", Color::Gray, 2);
	  // label_input_aux.setForecolor(Color::Gray);

    // UILabel label_input_aux({ 52, 0, 42, 18 }, "S/PDIF", Color::Gray, 2);
	  // label_input_aux.setForecolor(Color::Gray);

    //footer.Add(label_vol);
    footer.Add(label_out_spdif);
    footer.Add(label_out_aux);
    footer.Add(label_input_web);
    footer.Add(label_input_bt);
    footer.Add(label_input_mute);

    panel.Add(analyzer_panel);
    panel.Add(footer);
  
    AudioFrame frame = {0, 0};

    while (true)
    {
        // if (is_muted){
        //   label_input_mute.setBorderColor(Color::White);
        //   label_input_mute.setForecolor(Color::White);
        // } else {
        //   label_input_mute.setBorderColor(Color::Gray);
        //   label_input_mute.setForecolor(Color::Gray);
        // }

        for (int i = 0; i < SAMPLES; i++) 
        {            
            newTime = micros()-oldTime;
            oldTime = newTime;

            while (xQueueReceive(audioFrameQueue, &frame, portMAX_DELAY) == pdFALSE)
            {
                vTaskDelay(2);
            }

            vReal_l[i] = frame.left;
            vReal_r[i] = frame.right;

            vImag_l[i] = 0;
            vImag_r[i] = 0;

            while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
        }

        fft.Windowing(vReal_l, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        fft.Compute(vReal_l, vImag_l, SAMPLES, FFT_FORWARD);
        fft.ComplexToMagnitude(vReal_l, vImag_l, SAMPLES);

        // double peak = fft.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

        // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
        for (int band_index = 0, bin = 4; band_index < BANDS_COUNT; band_index++, bin+=4)
        {
            if (vReal_l[bin] < 400) { // Add a crude noise filter, 10 x amplitude or more
              continue;
            }
            analyzer.setBand(band_index, (int)vReal_l[bin] / 60);
        }

        // for (int band_index = 0, bin = 2;  bin < (SAMPLES/2); bin+=2)
        // { 
        //   if (vReal_l[i] < 400) { // Add a crude noise filter, 10 x amplitude or more
        //     continue;
        //   }

        //   if (band_index>=30) {
        //     break;
        //   }

        //   analyzer.setBand(band_index++, (int)vReal_l[bin] / 60); 

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

      // unsigned short l = frame.left + USHRT_MAX / 2.0;
      // unsigned short r = frame.right + USHRT_MAX / 2.0;

      // Serial.print(abs(frame.left));
      // Serial.print(" ");
      // Serial.println(abs(frame.right));

      level_left.SetValueOf(frame.left);
      level_right.SetValueOf(frame.right);

      // while (!level_left.IsValid() || !level_right.IsValid())
      // {
      //   panel.Update(canvas); 
      // }
      
      // if (50 < (millis() - time))
      // {
      //   unsigned short l = frame.left ;// + USHRT_MAX / 2.0;
      //   unsigned short r = frame.right;// + USHRT_MAX / 2.0;

      //   level_left.SetAnimatedValueOf(l);
      //   level_right.SetAnimatedValueOf(r);

      //   time = millis();

      //   vTaskDelay(2);
      // }

      panel.Update(*canvas);

      vTaskDelay(2);
    }

    vTaskDelete(NULL);
}

void startAnalyzer(void * args)
{
  xTaskCreatePinnedToCore(
                    main_analyzer,  /* Task function. */
                    "UI",           /* name of task. */
                    20000,          /* Stack size of task */
                    args,           /* parameter of the task */
                    1,              /* priority of the task */
                    &analyzerHandle,/* Task handle to keep track of created task */
                    0);             /* pin task to core 0 */                  
  delay(500);
}