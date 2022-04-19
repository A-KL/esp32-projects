#pragma once

#include <arduinoFFT.h>
#include "bands.h"

static TaskHandle_t uiHandle;
static xQueueHandle audioFrameQueue = xQueueCreate(SAMPLES, sizeof(AudioFrame));

void loopUI(void * args)
{
    auto canvas = *(TCanvas*)args;

    AudioFrame frame = {0, 0};

    arduinoFFT fft_left(vReal_l, vImag_l, SAMPLES, samplig_rate);
    arduinoFFT fft_right(vReal_r, vImag_r, SAMPLES, samplig_rate);

    while (true)
    {
        for (int i = 0; i < SAMPLES; i++) 
        {
            xQueueReceive(audioFrameQueue, &frame, pdMS_TO_TICKS(1));

            vReal_l[i] = frame.left;
            vReal_r[i] = frame.right;

            vImag_l[i] = 0;
            vImag_r[i] = 0;
        }

        fft_left.DCRemoval();
        fft_left.Windowing(FFT_WIN_TYP_HANN, FFT_FORWARD);
        fft_left.Compute(FFT_FORWARD);
        fft_left.ComplexToMagnitude();


        for (int band_index = 0; band_index < form.equalizer.bands.count(); band_index++)
        {
          bands[band_index].amplitude = 0;

          for (int bin = 1; bin < (SAMPLES/2); bin++)
          {
            if (bands[band_index].inRange(bin) && vReal_l[bin] > 500)
            {
              bands[band_index].amplitude  += (int)vReal_l[bin];
            }
          }
        }

        for (int band_index = 0; band_index < form.equalizer.bands.count(); band_index++)
        {
          auto value = bands[band_index].amplitude / form.equalizer.bands.maxBand();

          value = value > form.equalizer.bands.maxBand() ? form.equalizer.bands.maxBand() : value;

          value = (bands[band_index].amplitude_old + bands[band_index].amplitude) / 2;

          bands[band_index].amplitude_old = value;

          form.equalizer.bands.setBand(band_index, value);
        }
    
        //double max_f_left = 0;
        //double max_v_left = 0;

        //fft_left.MajorPeak(&max_f_left, &max_v_left);

        // // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
        // for (int band_index = 0, bin = 1; band_index < BANDS_COUNT; band_index++, bin+=4)
        // {
        //     if (vReal_l[bin] < 400) { // Add a crude noise filter, 10 x amplitude or more
        //       analyzer.setBand(band_index, 0);
        //       continue;
        //     }
        //     analyzer.setBand(band_index, (int)vReal_l[bin] % 60); // map((int)vReal_l[bin], 0, 3300, 0, 60);
        // }

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

        // Serial.print(abs(frame.left));
        // Serial.print(" ");
        // Serial.println(abs(frame.right));

        form.levelLeft.setValueOf((frame.left + form.levelLeft.valueOf()) / 2);
        form.levelRight.setValueOf((frame.right + form.levelRight.valueOf()) / 2);

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
    
      form.Update(canvas);

      vTaskDelay(pdMS_TO_TICKS(1));
    }

    vTaskDelete(NULL);
}

float binToFreq(int index, unsigned int sample_count, unsigned int samplig_rate = 44100)
{
  return index * samplig_rate / (float)sample_count;
}

void startUI(void * args)
{
  xTaskCreatePinnedToCore(
                    loopUI,  /* Task function. */
                    "UI",           /* name of task. */
                    20000,          /* Stack size of task */
                    args,           /* parameter of the task */
                    1,              /* priority of the task */
                    &uiHandle,/* Task handle to keep track of created task */
                    0);             /* pin task to core 0 */                  
  delay(500);
}