//#include <M5StickCPlus.h>
#include <math.h>
#include <driver/i2s.h>

#include <SPI.h>
#include <TFT_eSPI.h>

// Connections to INMP441 I2S microphone
#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
 
// Use I2S Processor 0
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 44100

#define READ_LEN    (2 * 256)
#define GAIN_FACTOR 30

#define BAR_W 20
#define BAR_H 135

TFT_eSPI tft = TFT_eSPI();

uint8_t bar_value = 0;

uint8_t BUFFER[READ_LEN] = {0};
uint16_t oldy[160];
int16_t *adcBuffer = NULL;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),  // Set the I2S operating mode.
    .sample_rate = I2S_SAMPLE_RATE,                // Set the I2S sampling rate.
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,   // Set the channel format.
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
        I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
    .dma_buf_count = 2,        // DMA buffer count.
    .dma_buf_len   = 128,      // DMA buffer length.
    .use_apll = false
};

void i2s_init() {
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

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
    //i2s_set_clk(I2S_NUM_0, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

float EnvelopeOut;
float envIn;
float attack = 0.996879878;
float release = 0.9996879878;
float DC_Filter = 0;
float X_Prev = 0;
float X= 0;

void showSignal() {
    int y;

    auto value =  map(EnvelopeOut, 0, INT16_MAX, 0, BAR_H);
    auto bx = TFT_WIDTH - BAR_W;
    auto by = TFT_HEIGHT - value;

    tft.fillRect(bx, 0, BAR_W, BAR_H, TFT_WHITE);
    tft.fillRect(bx, by, BAR_W, value, TFT_BLACK);

    for (int n = 0; n < 160; n++) {
        y = adcBuffer[n] * GAIN_FACTOR;
        y = map(y, INT16_MIN, INT16_MAX, 10, 70);

        tft.drawPixel(n, oldy[n], TFT_WHITE);
        tft.drawPixel(n, y, TFT_BLACK);

        oldy[n] = y;
    }
}

void mic_record_task(void *arg)  {
    size_t bytesread;

    while (1) {
        i2s_read(I2S_PORT, (char *)BUFFER, READ_LEN, &bytesread, (100 / portTICK_RATE_MS));

        adcBuffer = (int16_t *)BUFFER;

        for (auto i=0; i<bytesread/2; i++) {
            X = adcBuffer[i];

            X = X * 300;

	    	DC_Filter = X - X_Prev + (.99 * DC_Filter);

	    	X_Prev = X;

	    	envIn = fabs(DC_Filter);

    		 if(EnvelopeOut < envIn)
    		 {
    			 EnvelopeOut = envIn + attack * (EnvelopeOut - envIn);
    		 }
    		 else
    		 {
    			 EnvelopeOut = envIn + release * (EnvelopeOut - envIn);
    		 }
        }

        showSignal();

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

/* 
  After M5StickC is started or reset
  the program in the setUp () function will be run, and this part will only be run once. 在 M5StickC
  */
void setup() {
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.fillScreen(TFT_WHITE);

    attack = expf(-1.0/((float)I2S_SAMPLE_RATE * .050)); //50mS Attack
    release = expf(-1.0/((float)I2S_SAMPLE_RATE * .100)); //100mS Release

    i2s_init();
    i2s_start(I2S_PORT);

    xTaskCreate(mic_record_task, "mic_record_task", 2048, NULL, 1, NULL);
}

/* 
After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
*/
void loop() {
    printf("loop cycling\n");
    vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half of the cpu cycles
}