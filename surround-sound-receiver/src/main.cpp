#include <Arduino.h>
#include "Network.h"

#include <driver/i2s.h>

#include "espressif_logo.h"
#include "TFTCanvas.h"

#include "MainForm.h"

#include "i2s_adc.h"

TFTCanvas canvas;

MainForm form({ 0, 0, 320, 240 });

// // don't mess around with this
// i2s_config_t i2s_config = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//     .sample_rate = 44100,
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = I2S_COMM_FORMAT_I2S,
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//     .dma_buf_count = 4,
//     .dma_buf_len = 1024,
//     .use_apll = false,
//     .tx_desc_auto_clear = false,
//     .fixed_mclk = 0
//     };

// // and don't mess around with this
// i2s_pin_config_t i2s_mic_pins = {
//     .bck_io_num = I2S_MIC_SERIAL_CLOCK,
//     .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
//     .data_out_num = I2S_PIN_NO_CHANGE,
//     .data_in_num = I2S_MIC_SERIAL_DATA
//     };

#include "AudioTools.h"

#include "UvMeterI2S.h"

UvMeterI2S i2s;
//UvAvgMeterI2S i2s;
MultiOutput multi;

StreamCopy copier(multi, i2s);

uint16_t sample_rate = 44100;
uint16_t channels = 2;
uint16_t bits_per_sample = 16; 

TaskHandle_t vuTask;

// I2SStream i2sStream; // Access I2S as stream
// CsvStream<int32_t> csvStream(Serial);
// StreamCopy copier(csvStream, i2sStream); // copy i2sStream to csvStream

// // Arduino Setup
// void setup(void) {
//     Serial.begin(115200);
//     AudioLogger::instance().begin(Serial, AudioLogger::Info);
    
//     auto cfg = i2sStream.defaultConfig(RX_MODE);
//     cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
//     cfg.bits_per_sample = 32;
//     cfg.channels = 2;
//     cfg.sample_rate = 44100;
//     cfg.is_master = true;
//      // this module nees a master clock if the ESP32 is master
//     cfg.use_apll = false;  // try with yes
//     cfg.pin_mck = 3; 
//     i2sStream.begin(cfg);

//     // make sure that we have the correct channels set up
//     csvStream.begin(cfg);

// }

// // Arduino loop - copy data
// void loop() {
//     copier.copy();
// }

// float mean = 0;
// unsigned long started = 0;

uint16_t right = 0;
uint16_t left = 0;

void vu_task( void * parameter) {
  for (;;) {
    if (i2s.sampled) {
      i2s.sampled = false;
      // myData.l = i2s.left;
      // myData.r = i2s.right;
      // for (uint8_t f = 1; f < n_bins - 1; f++) {
      //   myData.m[f] = mag[f];
      // }
      //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(send_struct));

      if (i2s.left > left)
      {
        left += (i2s.left - left) * 0.30;
      }
      else
      {
        left -= (left - i2s.left) * 0.30;
      }

      if (i2s.right > right)
      {
        right += (i2s.right - right) * 0.30;
      }
      else
      {
        right -= (right - i2s.right) * 0.30;
      }

      // auto val = map(left, 61000, 65535, 0, 65535);
      // auto val2 = map(right, 61000, 65535, 0, 65535);

      Serial.println(left);

      form.levelLeft.setValueOf(i2s.left);
      form.levelRight.setValueOf(i2s.right);

      form.Update(canvas);
    }
    
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  
  canvas.Init(Color::White);
  canvas.SetFont(0, 1);
  canvas.DrawImage(0, 30, 320, 180, espressif_logo);

  delay(1000);

  AudioLogger::instance().begin(Serial, AudioLogger::Error);

  // i2s_install();
  // i2s_setpin();
  // i2s_start(I2S_PORT);

  // i2s_adc_install();

  // start I2S in
  Serial.println("Starting I2S");
  auto config = i2s.defaultConfig(RXTX_MODE);
  config.sample_rate = sample_rate;
  config.bits_per_sample = bits_per_sample;
  config.channels = 2;
  config.i2s_format = I2S_STD_FORMAT;
  config.pin_ws = 25;
  config.pin_bck = 27;
  config.pin_data = 26;
  config.pin_data_rx = 35;
  //config.fixed_mclk = 0;
  config.pin_mck = 0;
  i2s.begin(config);

  // Setup FFT
  // auto tcfg = fft.defaultConfig();
  // tcfg.length = 2 * n_bins;  // will produce n_bins frequency bins
  // tcfg.channels = channels;
  // tcfg.sample_rate = sample_rate;
  // tcfg.bits_per_sample = bits_per_sample;
  // tcfg.callback = &fftResult;
  // fft.begin(tcfg);

  multi.add(i2s);
 // multi.add(fft);
  multi.begin(config);

  canvas.Clear(Color::Black);

  Serial.println("I2S started...");
  xTaskCreatePinnedToCore(vu_task, "VU-spectrum-broadcast", 10000, NULL, 2, &vuTask, 1);
}

void loop(){
  copier.copy();
}

// void loop() {
//   // The 4 high bits are the channel, and the data is inverted
//   size_t bytes_read;
//   uint16_t buffer[I2S_DMA_BUF_LEN] = {0};

//   uint32_t read_counter = 0;
//   uint32_t averaged_reading = 0;
//   uint64_t read_sum = 0;

//   uint16_t left_current = 0;
//   uint16_t left_new = 0;

//   uint16_t left_min = 65535;
//   uint16_t left_max = 0;

//   while(1) 
//   {
//     esp_err_t result = i2s_read(I2S_NUM_0, &buffer, sizeof(buffer), &bytes_read, 15);

//     if (result == ESP_OK) 
//     {
//       for (int i = 0; i < bytes_read/2; ++i) 
//       {
//         //uint16_t value = buffer[i] & 0x0FFF;

//         read_sum += buffer[i] & 0x0FFF;
//         ++read_counter;

//         // if (value > left_max) 
//         // {
//         //   left_max = value;
//         // } 
//         // else if (value < left_min)
//         // {
//         //   left_min = value;
//         // }
//         // else 
//         // {
//         //   left_new = (left_max - left_min)  / 2510.0  * 65535;
//         //   left_min = 65535;
//         //   left_max = 0;

//         //   form.levelLeft.setValueOf(left_new);
//         // }

//         if (read_counter == AVERAGE_EVERY_N_SAMPLES)
//         {
//           averaged_reading = read_sum / AVERAGE_EVERY_N_SAMPLES;

//           read_counter = 0;
//           read_sum = 0;

//           Serial.println(averaged_reading); // Print compatible with Arduino Plotter
//           // Serial.printf("Display_signal:%d", d); // Print compatible with Arduino Plotter
        
//           left_new = averaged_reading  / 4000.0  * 65535;
//           // left_min = 65535;
//           // left_max = 0;

//           // if (left_new > left_current)
//           // {
//           //     left_current += (left_new - left_current) * 0.30;
//           // }

//           // if (left_new < left_current)
//           // {
//           //     left_current -= (left_current - left_new) * 0.30;
//           // }

//           form.levelLeft.setValueOf(left_new);
//         }
//       } // for
//     }
//     form.Update(canvas);
//   } // while

//   // // False print statements to "lock range" on serial plotter display
//   // // Change rangelimit value to adjust "sensitivity"
//   // int rangelimit = 16000;

//   // Serial.print(rangelimit * -1);
//   // Serial.print(" ");
//   // Serial.print(rangelimit);
//   // Serial.print(" ");
 
//   // // Get I2S data and place in data buffer
//   // size_t bytesIn = 0;

//   // esp_err_t result = i2s_read(I2S_PORT, &buffer, bufferLen, &bytesIn, portMAX_DELAY);
 
//   // if (result == ESP_OK)
//   // {
//   //   // Read I2S data buffer
//   //   int16_t samples_read = bytesIn / 8;
//   //   if (samples_read > 0) {
      
//   //     for (int16_t i = 0; i < samples_read; ++i) {
//   //       mean += (buffer[i]);
//   //     }
 
//   //     // Average the data reading
//   //     mean /= samples_read;
//   //   }
//   // }

//   // if (millis() - started > 200)
//   // {
//   //   started = millis();
//   //   form.levelLeft.setValueOf(mean * UCHAR_MAX);
 
//   //   Serial.println(mean);
//   //   mean = 0;
//   // }

//   form.Update(canvas);
// }