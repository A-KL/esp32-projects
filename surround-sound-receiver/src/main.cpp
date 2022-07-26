#include <Arduino.h>
#include "Network.h"

#include <driver/i2s.h>

#include "espressif_logo.h"
#include "Color.h"
#include "TFTCanvas.h"

#define I2S_PORT I2S_NUM_0
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32

#define bufferLen 64
int16_t buffer[bufferLen];

TFTCanvas canvas;

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

// #include "AudioTools.h"


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

void i2s_install() {
  const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = bufferLen,
    .use_apll = false
  };
 
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}
 
void i2s_setpin() {
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
   // .mck_io_num = I2S_PIN_NO_CHANGE,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };
 
  i2s_set_pin(I2S_PORT, &pin_config);
}

void setup() {
  Serial.begin(115200);
  
  canvas.Init(Color::Red);
  // canvas.SetFont(0, 1);
  // canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  // i2s_install();
  // i2s_setpin();
  // i2s_start(I2S_PORT);
 
  delay(5000);
}

void loop() {
  delay(5000);
  return;
  // False print statements to "lock range" on serial plotter display
  // Change rangelimit value to adjust "sensitivity"
  int rangelimit = 3000;

  Serial.print(rangelimit * -1);
  Serial.print(" ");
  Serial.print(rangelimit);
  Serial.print(" ");
 
  // Get I2S data and place in data buffer
  size_t bytesIn = 0;

  esp_err_t result = i2s_read(I2S_PORT, &buffer, bufferLen, &bytesIn, portMAX_DELAY);
 
  if (result == ESP_OK)
  {
    // Read I2S data buffer
    int16_t samples_read = bytesIn / 8;
    if (samples_read > 0) {
      float mean = 0;
      for (int16_t i = 0; i < samples_read; ++i) {
        mean += (buffer[i]);
      }
 
      // Average the data reading
      mean /= samples_read;
 
      Serial.println(mean);
    }
  }
}