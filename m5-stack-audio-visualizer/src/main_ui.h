#pragma once

// ---------------------------------------------------

#define DEBOUNCE_TIME 50

// ---------------------------------------------------
#define ADC_MIC           34
#define ADC_CHANNEL_LEFT  ADC_MIC
#define ADC_CHANNEL_RIGHT 35

#define SAMPLES 512              // Must be a power of 2

#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE_MAX 255

#define BANDS_COUNT 30

#define VOLUME_PIN_A  34
#define VOLUME_PIN_B  35
#define VOLUME_BUTTON 32

RadioStation Stations[] { 
  {"Mega Shuffle", "http://jenny.torontocast.com:8134/stream"},

  {"WayUp Radio", "http://188.165.212.154:8478/stream"},
  {"Asia Dream", "https://igor.torontocast.com:1025/;.mp3"},
  {"KPop Radio", "http://streamer.radio.co/s06b196587/listen"},

  {"Classic FM", "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites", "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out", "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas", "http://ice2.somafm.com/christmas-128-mp3"}
};

 void OnEncoderChanged(void* arg) {
  auto encoder = (ESP32Encoder*) arg;
  //reinterpret_cast<const char *>(cbData);
  //encoder->getCount();
 Serial.printf("enc cb: count: %d\n", 123);
}

// ---------------------------------------------------

static ESP32Encoder encoder(true, OnEncoderChanged);
static InternetRadio radio;
static arduinoFFT fft;
static TaskHandle_t analyzerHandle;
static xQueueHandle audioFrameQueue = xQueueCreate(SAMPLES, sizeof(AudioFrame));
UILabel label_track({ 30, 240-20, 200, 23 }, "Test", NULL, 16);
// ---------------------------------------------------

static void onButtonUp()
{
  Serial.println("OnButtonUp");
}

// ---------------------------------------------------

void onAudioFrameCallback(const AudioFrame& frame)
{
  xQueueSend(audioFrameQueue, &frame, 0);
}

void onStreamChanged(const char *type, const char *value)
{
  if (strcmp(type, "StreamTitle") == 0) { 
    label_track.SetText(value);
    Serial.println(value);
  }
}

void setupRadio()
{
  pinMode(VOLUME_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(VOLUME_BUTTON), onButtonUp, RISING );

  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachSingleEdge(VOLUME_PIN_A, VOLUME_PIN_B);
  encoder.clearCount();
  encoder.setFilter(1023);

  radio.Play(Stations[3].Url);
  radio.SampleCallback(onAudioFrameCallback);
  radio.StreamChanged = onStreamChanged;
}

void loopRadio()
{
    radio.Loop();
}

// ---------------------------------------------------

unsigned int sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
unsigned long newTime, oldTime, microseconds;

unsigned char peak[BANDS_COUNT];

double vReal_l[SAMPLES];
double vReal_r[SAMPLES];

double vImag_l[SAMPLES];
double vImag_r[SAMPLES];

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

// ---------------------------------------------------

void main_analyzer(void * args)
{
    memset(peak, 0, BANDS_COUNT);

    auto canvas = *(TFTCanvas*)args;

    // Root
    UIContainer panel({ 0, 0, 320, 240});

    // Analyzer
    UIContainer analyzer_panel({ 0, 0, 320, 240 - 23 });

    const char* font = NULL;
    UILabel label({ 0, 0, 320, 25 }, "S/PDIF", font, 16);

    auto start = 18;
    UILabel label_0({ 8, start, 18, 16 }, "0", font, 16);
    UILabel label_10({ 5, start += 19, 20, 16 }, "-10", font, 16);
    UILabel label_20({ 5, start += 20, 20, 16 }, "-20", font, 16);
    UILabel label_30({ 5, start += 19, 20, 16 }, "-30", font, 16);
    UILabel label_40({ 5, start += 20, 20, 16 }, "-40", font, 16);
    UILabel label_50({ 5, start += 19, 20, 16 }, "-50", font, 16);
    UILabel label_60({ 5, start += 20, 20, 16 }, "-60", font, 16);

	  UISoundAnalyzer<BANDS_COUNT> analyzer({ 30, 25, 270, 120 });

    UILabel level_left_label({ 0, 181, 20, 16 }, "L", NULL, 16);
    UILabel level_right_label({ 0, 181 + 13 + 3, 20, 16 }, "R", NULL, 16);

    UVAnimatedProgressOf<uint8_t> level_left({ 24, 181,           246, 15 }, 0, 4095, 4095 * 0.9, 0);
    UVAnimatedProgressOf<uint8_t> level_right({ 24, 181 + 15 + 3, 246, 15 }, 0, 4095, 4095 * 0.9, 0);

    level_left.Clear(canvas);
    level_right.Clear(canvas);

    analyzer_panel.Add(label);
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

    panel.Add(stations);

    // Footer
    UILabel label_vol({ 0, 240-20, 40, 23 }, "VOL:", font, 16);
    label_vol.Background = { 56, 56, 56, 0 };

    
    label_track.Background = { 56, 56, 56, 0 };

    UIContainer footer({ 0, 240-23, 320, 23 });
    footer.Background = { 56, 56, 56, 0 };

    footer.Clear(canvas);

   // panel.Add(label_vol);
    panel.Add(label_track);
    panel.Add(footer);

    panel.Add(analyzer_panel);
  
    AudioFrame frame;

    while (true)
    {
        unsigned long sum_l = 0;
        unsigned long sum_r = 0;

        for (int i = 0; i < SAMPLES; i++) 
        {
            newTime = micros()-oldTime;
            oldTime = newTime;

            while (xQueueReceive(audioFrameQueue, &frame, portMAX_DELAY) == pdFALSE)
            {
                vTaskDelay(5);
            }
            
            vReal_l[i] = frame.left;
            vReal_r[i] = frame.right;

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

      // auto avg_l = sum_l / (float)SAMPLES;
      // auto avg_r = sum_r / (float)SAMPLES;

      unsigned short l = frame.left + USHRT_MAX / 2.0;
      unsigned short r = frame.right + USHRT_MAX / 2.0;

      // Serial.print(abs(frame.left));
      // Serial.print(" ");
      // Serial.println(abs(frame.right));

      vTaskDelay(3);

      level_left.SetValueOf(l);
      level_right.SetValueOf(r);


      // if (300 < (millis() - time))
      // {
      //   level_left.SetAnimatedValueOf(abs((sum_l / (float)SAMPLES))-1950);
      //   level_right.SetAnimatedValueOf(abs((sum_r / (float)SAMPLES)-1950));
      //   time = millis();
      // }

      // while(!level_left.IsValid() || !level_right.IsValid())
      // {
      //   level_left.Draw(canvas);
      //   level_right.Draw(canvas);
      //   vTaskDelay(3);
      // }

      panel.Draw(canvas);
    }
    vTaskDelete(NULL);
}

void startAnalyzer(void * args)
{
  xTaskCreatePinnedToCore(
                    main_analyzer,  /* Task function. */
                    "UI",           /* name of task. */
                    10000,          /* Stack size of task */
                    args,           /* parameter of the task */
                    1,              /* priority of the task */
                    &analyzerHandle,/* Task handle to keep track of created task */
                    0);             /* pin task to core 0 */                  
  delay(500);
}