#include <M5StickCPlus.h>
#include <network.h>
#include <driver/i2s.h>

// you shouldn't need to change these settings
#define SAMPLE_BUFFER_SIZE 512

// either wire your microphone to the same pins or change these to match your wiring
#define I2S_MIC_SERIAL_CLOCK GPIO_NUM_0
#define I2S_MIC_LEFT_RIGHT_CLOCK -1
#define I2S_MIC_SERIAL_DATA GPIO_NUM_34

// don't mess around with this
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S), //I2S_COMM_FORMAT_I2S_MSB
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
    };

// and don't mess around with this
i2s_pin_config_t i2s_mic_pins = {
    .bck_io_num = I2S_MIC_SERIAL_CLOCK,
    .ws_io_num = I2S_MIC_LEFT_RIGHT_CLOCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_MIC_SERIAL_DATA
    };

int32_t raw_samples[SAMPLE_BUFFER_SIZE];

void setup() {
  Serial.begin(115200);

  M5.begin();

  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setRotation(1);
  //M5.Lcd.drawCentreString("C BACK Driver", 120, 30, 4);
  M5.Lcd.setTextFont(2);

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &i2s_mic_pins);

  //auto x = M5.IMU.Init();

  //wifi_init("m5stick-car");
}

// float pitch, roll, yaw;
// float temp;

void loop() {
  // M5.IMU.getTempData(&temp);
  // M5.Lcd.setCursor(5, 5);
  // M5.Lcd.printf("Temperature: %.2fC", (temp - 32) * 5.0/9);

  // M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  // M5.Lcd.setCursor(5, 45);
  // M5.Lcd.printf("X:%5.2f/nY:%5.2f/nZ:%5.2f ", pitch, roll, yaw);

  // read from the I2S device
  size_t bytes_read = 0;
  i2s_read(I2S_NUM_0, raw_samples, sizeof(int32_t) * SAMPLE_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);
  // dump the samples out to the serial channel.
  for (int i = 0; i < samples_read; i++)
  {
    Serial.printf("%ld\n", raw_samples[i]);
  }
}