#include <limits.h>
#include <Arduino.h>
#include <arduinoFFT.h>

#include "Network.h"
#include "Radio.h"

#include "Color.h"
#include "Canvas.h"

#define BANDS_COUNT 30 

#ifdef M5STACK
  #include "M5StackCanvas.h"
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
#endif

#include "UI.h"
#include "main_ui.h"

// ---------------------------------------------------
#define ADC_MIC          34
#define ADC_CHANNEL_LEFT ADC_MIC
#define ADC_CHANNEL_RIGHT 35

#define SAMPLES 512              // Must be a power of 2

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

const char* urls[] = {
  "http://jenny.torontocast.com:8134/stream",
  
  "http://188.165.212.154:8478/stream",
  "https://igor.torontocast.com:1025/;.mp3",
  "http://streamer.radio.co/s06b196587/listen",
   
  "http://media-ice.musicradio.com:80/ClassicFMMP3",
  "http://naxos.cdnstream.com:80/1255_128",
  "http://149.56.195.94:8015/steam",
  "http://ice2.somafm.com/christmas-128-mp3"
};

const char* station[] = {
  "Mega Shuffle",
 
  "WayUp Radio",
  "Asia Dream",
  "KPop Radio",
  
  "Classic FM",
  "Lite Favorites",
  "MAXXED Out",
  "SomaFM Xmas"
};

const int stations_count = sizeof(urls)/sizeof(char *);

// ---------------------------------------------------
arduinoFFT fft;

#ifdef M5STACK
  M5StackCanvas canvas;
#endif

#ifdef ESP_WROVER
  TFTCanvas canvas;
#endif

InternetRadio radio;
TaskHandle_t ui_task;
TaskHandle_t player_task;

typedef struct __attribute__((packed)) {
    int16_t left;
    int16_t right;
} my_data_t;

static xQueueHandle _queue = xQueueCreate(10, sizeof(my_data_t));


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

void OnSampleCallback(int16_t left, int16_t right)
{
  // level_left.SetFinal(map(left, SHRT_MIN, SHRT_MAX, 0, UCHAR_MAX));
  // level_right.SetFinal(map(right, SHRT_MIN, SHRT_MAX, 0, UCHAR_MAX));
}

void run_player( void * pvParameters )
{
  radio.StartPlaying(urls[3]);
  //radio.OnSampleCallback(OnSampleCallback);

  while (true)
  {
    radio.Loop();
  }
}

void setup() 
{
  Serial.begin(115200);

  canvas.Init(Color(0, 0, 0));

  memset(peak, 0, BANDS_COUNT);

  StartWifi();

  // xTaskCreatePinnedToCore(
  //                   run_ui,         /* Task function. */
  //                   "UI",           /* name of task. */
  //                   10000,          /* Stack size of task */
  //                   (void*)&canvas,/* parameter of the task */
  //                   1,              /* priority of the task */
  //                   &ui_task,       /* Task handle to keep track of created task */
  //                   0);             /* pin task to core 0 */                  
  // delay(500);

  // xTaskCreatePinnedToCore(
  //                   run_player,    /* Task function. */
  //                   "Player",        /* name of task. */
  //                   16000,           /* Stack size of task */
  //                   NULL,            /* parameter of the task */
  //                   1,               /* priority of the task */
  //                   &player_task,    /* Task handle to keep track of created task */
  //                   1);              /* pin task to core 0 */                  
  delay(500);

  run_player(NULL);

  // while (true)
	// {
    // unsigned long sum_l = 0;
    // unsigned long sum_r = 0;

    // for (int i = 0; i < SAMPLES; i++) {

    //   newTime = micros()-oldTime;
    //   oldTime = newTime;

    //   vReal_l[i] = 1110; //analogRead(ADC_CHANNEL_LEFT); // A conversion takes about 1uS on an ESP32
    //   vReal_r[i] = 1230;// analogRead(ADC_CHANNEL_RIGHT);

    //   vImag_l[i] = 0;
    //   vImag_r[i] = 0;

    //   sum_l += vReal_l[i];
    //   sum_r += vReal_r[i];

    //   while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
    // }

    // fft.Windowing(vReal_l, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    // fft.Compute(vReal_l, vImag_l, SAMPLES, FFT_FORWARD);
    // fft.ComplexToMagnitude(vReal_l, vImag_l, SAMPLES);

   // double peak = fft.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
    // for (int band_index = 0, bin = 2; band_index < BANDS_COUNT; band_index++, bin+=4)
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


    //level_left.SetFinal(abs((sum_l / (float)SAMPLES) - 2000));
   // level_right.SetFinal(abs((sum_r / (float)SAMPLES) - 1980));

    //panel.Draw(canvas);
	//}
}

void loop() {

}