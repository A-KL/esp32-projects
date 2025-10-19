#pragma once

#include <arduinoFFT.h>

// class FTT {
//     public:
//         FTT(const uint16_t samples = 512, uint16_t samplig_rate = 44100) : 
//             _samples(samples), _samplig_rate(samplig_rate), _vReal(NULL), _vImag(NULL) _current(0), _mutex(xSemaphoreCreateMutex())
//         {
//             xSemaphoreTake(_mutex);

//             _vReal = &vReal1;
//             _vImag1 = &vImag1;
//         }

//         inline void push(int16_t left, int16_t right)
//         {
//             xSemaphoreTake(_mutex, portMAX_DELAY);

//             _vReal[_counter] = left;
//             _vImag[_counter] = 0;

//             _counter++;

//             if (_counter == _samples)
//             {
//                 _counter = 0;

//                 _vReal = _vReal == &vReal1 ? &vReal2 : &vReal1;
//                 _vImag = _vImag == &vImag1 ? &vImag2 : &vImag1;

//                 xSemaphoreGive(_mutex);
//             }   
//         }

//         inline void wait()
//         {
//             xSemaphoreTake(_mutex, portMAX_DELAY);

//             auto r = _vReal;
//             auto i = _vImag;

//             xSemaphoreGive(_mutex);

//             arduinoFFT fft_left(r, i, _samples, _samplig_rate);

//             fft_left.DCRemoval();
//             fft_left.Windowing(FFT_WIN_TYP_HANN, FFT_FORWARD);
//             fft_left.Compute(FFT_FORWARD);
//             fft_left.ComplexToMagnitude();
//         }

//     private:
//         const uint16_t _samples;
//         const uint16_t _samplig_rate;

//         volatile double* _vReal, _vImag;
//         volatile int _counter;

//         double vReal1[512];
//         double vImag1[512];

//         double vReal2[512];
//         double vImag2[512];

//         SemaphoreHandle_t  _mutex;
// };