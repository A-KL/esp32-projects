#include <math.h>
#include <driver/i2s.h>

#include "gui.h"
#include "envelope_detector.h"

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
 
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 44100

#define I2S_SAMPLES_PER_MS I2S_SAMPLE_RATE / 1000

audio_envelope_context_t right_envelope_context;
audio_envelope_context_t left_envelope_context;

//audio_envelope_context_t envelope_context;

#define I2S_BUFFER_SIZE 128

uint8_t sample_size = sizeof(int32_t); // int16_t

int32_t samples[I2S_BUFFER_SIZE]; // int16_t

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),  // | I2S_MODE_PDM
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, //I2S_CHANNEL_FMT_ONLY_LEFT
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
        I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
    .dma_buf_count = 8,                        // DMA buffer count.
    .dma_buf_len   = I2S_BUFFER_SIZE,          // DMA buffer length.
    .use_apll      = false
};

const i2s_config_t i2s_rx_config(i2s_mode_t mode = (i2s_mode_t)0, uint16_t sample_rate = 44100, uint16_t buffer_size = 128)
{
    return i2s_config_t
    {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | mode),  // I2S_MODE_PDM
        .sample_rate = sample_rate,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    #if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
        .communication_format =
            I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
    #else
        .communication_format = I2S_COMM_FORMAT_I2S,
    #endif
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
        .dma_buf_count = 8,                        // DMA buffer count.
        .dma_buf_len   = buffer_size,              // DMA buffer length.
        .use_apll      = false
    };
}

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

    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.loadFont(NotoSansBold15);

    tft.fillScreen(TFT_BLACK);

    gui_init();

    envelope_init(right_envelope_context, I2S_SAMPLE_RATE);
    envelope_init(left_envelope_context, I2S_SAMPLE_RATE);

    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);

    xTaskCreate(gui_analogread_task, "gui_task", 2048, NULL, 1, NULL);
}

void loop() 
{
    auto rangelimit = 3000;

    Serial.print(rangelimit * -1);
    Serial.print(" ");
    Serial.print(rangelimit);
    Serial.print(" ");

    size_t bytes_read = 0;
    esp_err_t result = i2s_read(I2S_PORT, &samples, I2S_BUFFER_SIZE, &bytes_read, portMAX_DELAY);

    if (result == ESP_OK)
    {
        int16_t samples_read = bytes_read / sample_size;
        
        if (samples_read > 0) 
        {
            float mean = 0;

            for (int16_t i = 0; i < samples_read; ++i) 
            {
                mean += ((int16_t)samples[i]);
            }
    
            // Average the data reading
            mean /= samples_read;
        
            envelope_calculate_right_left(samples, samples_read, right_envelope_context, left_envelope_context);

            Serial.print(left_envelope_context.envelope_out);

            Serial.print(" ");

            Serial.print(right_envelope_context.envelope_out);

            Serial.print(" ");

            Serial.println(mean);

            left_pb.value = left_envelope_context.envelope_out;
            right_pb.value = right_envelope_context.envelope_out;
        }  
    }
    // printf("loop cycling\n");
    // vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half of the cpu cycles
}