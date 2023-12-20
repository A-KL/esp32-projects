//#include <M5StickCPlus.h>
#include <math.h>
#include <driver/i2s.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "gui_progressbar.h"
#include "gui_led.h"
#include "Orbitron_Medium_20.h"

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

TFT_eSprite left_pb = TFT_eSprite(&tft); 
TFT_eSprite right_pb = TFT_eSprite(&tft);
TFT_eSprite main_led_sprite = TFT_eSprite(&tft);
TFT_eSprite second_led_sprite = TFT_eSprite(&tft);

gui_progressbar_t gui_left_pb;
gui_progressbar_t gui_right_pb;

gui_led_t main_led;
gui_led_t second_led;

float EnvelopeOut;
float envIn;
float attack = 0.996879878;
float release = 0.9996879878;
float DC_Filter = 0;
float X_Prev = 0;
float X= 0;

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

       // showSignal();

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void gui_init() {
    main_led.top = 100;
    main_led.left = 15;
    main_led.value = true;

    second_led.top = 100;
    second_led.left = 40;
    second_led.value = true;
    second_led.on_color = TFT_RED;
    second_led.on_color_to = right_pb.color24to16(0xCC0000);
    second_led.off_color = right_pb.color24to16(0x880000); // DARK_RED

    gui_left_pb.top = 10;
    gui_left_pb.left = 15;
    gui_left_pb.width = 200;
    gui_left_pb.max = 4095;
    gui_left_pb.background_color = TFT_DARK_DARK_GRAY;
    gui_left_pb.value_style = &pb_lime_segmented_style;

    gui_right_pb.top = 40;
    gui_right_pb.left = 15;
    gui_right_pb.width = 200;
    gui_right_pb.max = 4095;
    gui_right_pb.background_color = TFT_DARKGREEN;
    gui_right_pb.borders_thickness[0] = 1;
    gui_right_pb.borders_thickness[1] = 1;
    gui_right_pb.borders_thickness[2] = 1;
    gui_right_pb.borders_thickness[3] = 1;  

    gui_led_init(main_led_sprite, main_led);
    gui_led_init(second_led_sprite, second_led);
    gui_progressbar_init(left_pb, gui_left_pb);
    gui_progressbar_init(right_pb, gui_right_pb);

    gui_led_begin(main_led_sprite, main_led);
    gui_led_begin(second_led_sprite, second_led);
    gui_progressbar_begin(left_pb, gui_left_pb);
    gui_progressbar_begin(right_pb, gui_right_pb); 
}

void setup() {
    tft.init();
    tft.setRotation(1);
    tft.setSwapBytes(true);
    tft.setFreeFont(&Orbitron_Medium_20);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(15, 80);
    tft.printf("Value: %d", 200);

    gui_init();


    attack = expf(-1.0/((float)I2S_SAMPLE_RATE * .050)); //50mS Attack
    release = expf(-1.0/((float)I2S_SAMPLE_RATE * .100)); //100mS Release

    i2s_init();
    i2s_start(I2S_PORT);

   // xTaskCreate(mic_record_task, "mic_record_task", 2048, NULL, 1, NULL);
}

void loop() {
    // printf("loop cycling\n");
    // vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half of the cpu cycles

    auto left = analogRead(12);
    auto right = analogRead(13);

    gui_left_pb.value = left;
    gui_right_pb.value = right;

    main_led.value = left > 2000;
    second_led.value = right > 500;

    gui_progressbar_update(left_pb, gui_left_pb);
    gui_progressbar_update(right_pb, gui_right_pb);

    gui_led_update(main_led_sprite, main_led);
    gui_led_update(second_led_sprite, second_led);

    delay(100);
}