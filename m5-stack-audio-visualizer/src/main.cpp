#include <limits.h>
#include <Arduino.h>
#include <arduinoFFT.h>
#include <M5GFX.h>

#include "Color.h"
#include "Canvas.h"
#include "M5StackCanvas.h"

#include "UI.h"

// ---------------------------------------------------
#define ADC_PIN ADC2
#define SAMPLES 512              // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.

struct eqBand {
  const char *freqname;
  uint16_t amplitude;
  int peak;
  int lastpeak;
  uint16_t lastval;
  unsigned long lastmeasured;
};
 
eqBand audiospectrum[9] = {
  //Adjust the amplitude values to fit your microphone
  { "125Hz", 500, 0, 0, 0, 0},
  { "250Hz", 200, 0, 0, 0, 0},
  { "500Hz", 200, 0, 0, 0, 0},
  { "1KHz",  200, 0, 0, 0, 0},
  { "2KHz",  200, 0, 0, 0, 0},
  { "4KHz",  100, 0, 0, 0, 0},
  { "6k3",   100, 0, 0, 0, 0},
  { "10KHz", 100, 0, 0, 0, 0},
  { "16KHz", 50,  0, 0, 0, 0}
};

unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
uint16_t tft_width  = 320; // ILI9341_TFTWIDTH;
uint16_t tft_height = 240; // ILI9341_TFTHEIGHT;
uint8_t bands = 8;
uint8_t bands_width = floor( tft_width / bands );
uint8_t bands_pad = bands_width - 10;
uint16_t colormap[255]; // color palette for the band meter (pre-fill in setup)

// ---------------------------------------------------

void displayBand(int band, int dsize)
{
  int dmax = 50;

  if (dsize > dmax) 
  	dsize = dmax;

//   if (band == 7) 
//   	display.drawHorizontalLine(18*6,0, 14);

//   for (int s = 0; s <= dsize; s=s+2)
//   	display.drawHorizontalLine(18*band,64-s, 14);

  if (dsize > peak[band]) 
  	peak[band] = dsize;
}

void setup() {
  M5GFX display;
  M5StackCanvas canvas(display);

  arduinoFFT fft;
	sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

  canvas.Init(Color(0, 0, 0));

  display.waitDisplay();

  // UI
  UIContainer panel({ 0, 0, 320, 240 });

  // Title
  const char* font = NULL;

  UILabel label({ 0, 0, 320, 25 }, "S/PDIF", font, 20);

  // Main
	auto start = 20;
	UILabel label_0({ 14, start, 24, 13 }, "0", font, 16);
	UILabel label_10({ 17, start += 19, 24, 13 }, "-10", font, 16);
	UILabel label_20({ 17, start += 20, 24, 13 }, "-20", font, 16);
	UILabel label_30({ 17, start += 19, 24, 13 }, "-30", font, 16);
	UILabel label_40({ 17, start += 20, 24, 13 }, "-40", font, 16);
	UILabel label_50({ 17, start += 19, 24, 13 }, "-50", font, 16);
	UILabel label_60({ 17, start += 20, 24, 13 }, "-60", font, 16);

	UISoundAnalyzer<30> analyzer({ 30, 25, 270, 120 });

  // Levels
  UILabel level_left_label({ 0, 181, 24, 13 }, "L", font, 16);
  UILabel level_right_label({ 0, 181 + 13 + 3, 24, 13 }, "R", font, 16);

  UVProgress<uint16_t> level_left({ 24, 181, 244, 13 }, 0, 4095, 4095 * 0.9, 0);
  UVProgress<uint16_t> level_right({ 24, 181 + 13 + 3, 244, 13 }, 2000, 4095, 4095 * 0.9, 0);

  // Footer
  UIContainer footer({ 0, 240-23, 320, 23 });
  footer.Background = { 56, 56, 56, 0 };
  footer.Clear(canvas);

  //  Build UI
	panel.Add(label);
	panel.Add(label_0);
	panel.Add(label_10);
	panel.Add(label_20);
	panel.Add(label_30);
	panel.Add(label_40);
	panel.Add(label_50);
	panel.Add(label_60);
  panel.Add(level_left);
  panel.Add(level_right);
  panel.Add(level_left_label);
  panel.Add(level_right_label);
  panel.Add(footer);

  uint16_t l_value = 0;
  uint16_t r_level = 0;

  while (true)
	{
		for (int i = 0; i < SAMPLES; i++) 
		{
			newTime = micros()-oldTime;
			oldTime = newTime;

			l_value = analogRead(ADC1); // A conversion takes about 1uS on an ESP32

			vReal[i] = l_value;
			vImag[i] = 0;

			while (micros() < (newTime + sampling_period_us));
		}

		fft.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
		fft.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
		fft.ComplexToMagnitude(vReal, vImag, SAMPLES);

  for (int i = 2; i < (SAMPLES/2); i++){ 
    // Don't use sample 0 and only first SAMPLES/2 are usable. 
    // Each array eleement represents a frequency and its value the amplitude.
    if (vReal[i] > 1500) { // Add a crude noise filter, 10 x amplitude or more
      byte bandNum = getBand(i);
      if(bandNum!=8) {
        displayBand(bandNum, (int)vReal[i]/audiospectrum[bandNum].amplitude);
      }
    }
  }

  long vnow = millis();
  for (byte band = 0; band <= 7; band++) {
    // auto decay every 50ms on low activity bands
    if(vnow - audiospectrum[band].lastmeasured > 50) {
      displayBand(band, audiospectrum[band].lastval>4 ? audiospectrum[band].lastval-4 : 0);
    }
    if (audiospectrum[band].peak > 0) {
      audiospectrum[band].peak -= 2;
      if(audiospectrum[band].peak<=0) {
        audiospectrum[band].peak = 0;
      }
    }
    // only draw if peak changed
    if(audiospectrum[band].lastpeak != audiospectrum[band].peak) {
      // delete last peak
     M5.Lcd.drawFastHLine(bands_width*band,tft_height-audiospectrum[band].lastpeak,bands_pad,BLACK);
     audiospectrum[band].lastpeak = audiospectrum[band].peak;
     M5.Lcd.drawFastHLine(bands_width*band, tft_height-audiospectrum[band].peak,
                           bands_pad, colormap[tft_height-audiospectrum[band].peak]);
    }
  } 

		level_left.Set(l_value);
		level_right.Set(r_level);

		panel.Draw(canvas);
		delay(10);
	}
}

void displayBand(int band, int dsize){
  uint16_t hpos = bands_width*band;
  int dmax = 200;
  if(dsize>tft_height-10) {
    dsize = tft_height-10; // leave some hspace for text
  }
  // if(dsize < audiospectrum[band].lastval) {
  //   // lower value, delete some lines
  //   M5.Lcd.fillRect(hpos, tft_height-audiospectrum[band].lastval,
  //                   bands_pad, audiospectrum[band].lastval - dsize, BLACK);
  // }
  // if (dsize > dmax) dsize = dmax;
  // for (int s = 0; s <= dsize; s=s+4){
  //   M5.Lcd.drawFastHLine(hpos, tft_height-s, bands_pad, colormap[tft_height-s]);
  // }
  // if (dsize > audiospectrum[band].peak) {
  //   audiospectrum[band].peak = dsize;
  // }
  // audiospectrum[band].lastval = dsize;
  // audiospectrum[band].lastmeasured = millis();
}

byte getBand(int i) {
  if (i<=2 )             return 0;  // 125Hz
  if (i >3   && i<=5 )   return 1;  // 250Hz
  if (i >5   && i<=7 )   return 2;  // 500Hz
  if (i >7   && i<=15 )  return 3;  // 1000Hz
  if (i >15  && i<=30 )  return 4;  // 2000Hz
  if (i >30  && i<=53 )  return 5;  // 4000Hz
  if (i >53  && i<=200 ) return 6;  // 8000Hz
  if (i >200           ) return 7;  // 16000Hz
  return 8;
}

void loop() {
  // put your main code here, to run repeatedly:
}