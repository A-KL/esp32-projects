
// #include <list>
// #include <Arduino.h>
// #include <M5Stack.h>
#include <limits.h>
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
#define amplitude 200            // Depending on your audio source level, you may need to increase this value
unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;

// ---------------------------------------------------

M5GFX display;
M5StackCanvas canvas(display);

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

void setup(void)
{ 
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
	UIRect rect3{ 30, 25, 270, 120 };
	UISoundAnalyzer<30> analyzer(rect3);

	// Levels
	UILabel level_left_label({ 0, 181, 24, 13 }, "L", font, 16);
	UILabel level_right_label({ 0, 181 + 13 + 3, 24, 13 }, "R", font, 16);

	UVProgress<uint16_t> level_left({ 24, 181, 244, 13 }, 0, 4095, 4095 * 0.9, 0);
	UVProgress<uint16_t> level_right({ 24, 181 + 13 + 3, 244, 13 }, 0, 4095, 4095 * 0.9, 0);

	// Footer
	UIContainer footer({ 0, 240-23, 320, 23 });
	footer.Background = { 56, 56, 56, 0 };
	footer.Clear(canvas);

	//  Build UI
	panel.Add(label);
	panel.Add(analyzer);
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

			r_level = analogRead(ADC1); // A conversion takes about 1uS on an ESP32

			vReal[i] = r_level;
			vImag[i] = 0;

			while (micros() < (newTime + sampling_period_us));
		}

		r_level = analogRead(ADC2);

		fft.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
		fft.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
		fft.ComplexToMagnitude(vReal, vImag, SAMPLES);

		for (int i = 2; i < (SAMPLES/2); i++) { 
			// Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.

			if (vReal[i] > 2000) { // Add a crude noise filter, 10 x amplitude or more
			if (i<=2 )             displayBand(0,(int)vReal[i]/amplitude); // 125Hz
			if (i >3   && i<=5 )   displayBand(1,(int)vReal[i]/amplitude); // 250Hz
			if (i >5   && i<=7 )   displayBand(2,(int)vReal[i]/amplitude); // 500Hz
			if (i >7   && i<=15 )  displayBand(3,(int)vReal[i]/amplitude); // 1000Hz
			if (i >15  && i<=30 )  displayBand(4,(int)vReal[i]/amplitude); // 2000Hz
			if (i >30  && i<=53 )  displayBand(5,(int)vReal[i]/amplitude); // 4000Hz
			if (i >53  && i<=200 ) displayBand(6,(int)vReal[i]/amplitude); // 8000Hz
			if (i >200           ) displayBand(7,(int)vReal[i]/amplitude); // 16000Hz
			//Serial.println(i);
			}
		}

		level_left.Set(r_level);
		level_right.Set(r_level);

		panel.Draw(canvas);
		delay(10);
	}
}

void loop(void)
{ 


//     // for (byte band = 0; band <= 6; band++) 
// 	// 	display.drawHorizontalLine(18*band,64-peak[band],14);
//   }

//   if (millis()%4 == 0)
// 	  for (byte band = 0; band <= 6; band++)
// 		if (peak[band] > 0) 
// 				peak[band] -= 1;

  delay(10);
}