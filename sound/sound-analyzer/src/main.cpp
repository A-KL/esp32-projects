#include <math.h>
#include <driver/i2s.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "analyzer.h"

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
 
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 44100

#define I2S_SAMPLES_PER_MS 44100 / 1000
#define I2S_BUFFER_SIZE 128

int16_t samples[I2S_BUFFER_SIZE];

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),  // | I2S_MODE_PDM
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
    .dma_buf_count = 8,        // DMA buffer count.
    .dma_buf_len   = I2S_BUFFER_SIZE,      // DMA buffer length.
    .use_apll      = false
};

void i2s_install() {
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
}

void i2s_setpin() {
    const i2s_pin_config_t pin_config = {
    #if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
        .mck_io_num = I2S_PIN_NO_CHANGE,
    #endif
        .bck_io_num   = I2S_SCK,
        .ws_io_num    = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num  = I2S_SD
    };

    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() 
{
    Serial.begin(115200);
    Serial.println(" ");

    delay(1000);

    tft.init();
    tft.setRotation(1);
    tft.setSwapBytes(true);

    tft.fillScreen(TFT_BLACK);

    // i2s_install();
    // i2s_setpin();
    // i2s_start(I2S_PORT);

    //pinMode(clip_pin, OUTPUT);
    
    // Generate kernels (memory-intensive!) before starting any other tasks
    kernels = generate_kernels(cq_cfg);
    kernels = reallocate_kernels(kernels, cq_cfg);

    disableCore0WDT(); // disable the watchdog in order to let screen_Task_routine spin-wait
    
    xTaskCreatePinnedToCore(screen_Task_routine, "screen", 2500, NULL, configMAX_PRIORITIES-1, new TaskHandle_t, 0);
    xTaskCreatePinnedToCore(comp_Task_routine, "comp", 2500, NULL, configMAX_PRIORITIES-1, new TaskHandle_t, 1);
}

void loop() {
    vTaskSuspend(NULL); // suspend the arduino loop
}