#include <math.h>
#include <driver/i2s.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "Orbitron_Medium_20.h"
#include "Orbitron_Bold_12.h"
#include "NotoSansBold15.h"

#include "gui_progressbar.h"
#include "gui_led.h"

#include "audio.h"

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
 
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 44100

#define I2S_SAMPLES_PER_MS 44100 / 1000

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite left_pb_canvas = TFT_eSprite(&tft); 
TFT_eSprite right_pb_canvas = TFT_eSprite(&tft); 

TFT_eSprite main_led_sprite = TFT_eSprite(&tft);
TFT_eSprite second_led_sprite = TFT_eSprite(&tft);

gui_progressbar_t left_pb;
gui_progressbar_t right_pb;

gui_led_t main_led;
gui_led_t second_led;

audio_envelope_context_t envelope_context;

#define I2S_BUFFER_SIZE 128

int16_t samples[I2S_BUFFER_SIZE];

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),  // | I2S_MODE_PDM
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
        I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
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
    //i2s_set_clk(I2S_NUM_0, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void showSignal() {
    // int y;

    // auto value =  map(EnvelopeOut, 0, INT16_MAX, 0, BAR_H);
    // auto bx = TFT_WIDTH - BAR_W;
    // auto by = TFT_HEIGHT - value;

    // tft.fillRect(bx, 0, BAR_W, BAR_H, TFT_WHITE);
    // tft.fillRect(bx, by, BAR_W, value, TFT_BLACK);

    // for (int n = 0; n < 160; n++) {
    //     y = adcBuffer[n] * GAIN_FACTOR;
    //     y = map(y, INT16_MIN, INT16_MAX, 10, 70);

    //     tft.drawPixel(n, oldy[n], TFT_WHITE);
    //     tft.drawPixel(n, y, TFT_BLACK);

    //     oldy[n] = y;
    // }
}

void mic_record_task(void *arg)  {
    size_t bytesread;

    while (1) {
       // i2s_read(I2S_PORT, (char *)BUFFER, READ_LEN, &bytesread, (100 / portTICK_RATE_MS));

       // adcBuffer = (int16_t *)BUFFER;

        // for (auto i=0; i<bytesread/2; i++) {
        //     X = adcBuffer[i];

        //     X = X * 300;

	    // 	DC_Filter = X - X_Prev + (.99 * DC_Filter);

	    // 	X_Prev = X;

	    // 	envIn = fabs(DC_Filter);

    	// 	 if(EnvelopeOut < envIn)
    	// 	 {
    	// 		 EnvelopeOut = envIn + attack * (EnvelopeOut - envIn);
    	// 	 }
    	// 	 else
    	// 	 {
    	// 		 EnvelopeOut = envIn + release * (EnvelopeOut - envIn);
    	// 	 }
        // }

       // showSignal();

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void gui_init() {
    main_led.top = 100;
    main_led.left = 15;
    main_led.value = true;
    main_led.canvas = &main_led_sprite;

    second_led.top = 100;
    second_led.left = 40;
    second_led.value = true;
    second_led.canvas = &second_led_sprite;
    second_led.on_color = TFT_RED;
    second_led.on_color_to = TFT_DARK_RED_12;
    second_led.off_color = TFT_DARK_RED_8;

    left_pb.top = 10;
    left_pb.left = 15;
    left_pb.width = 200;
    left_pb.max = 1200;// 4095;
    left_pb.canvas = &left_pb_canvas;
    left_pb.background_color = TFT_BLACK; //TFT_DARK_DARK_GRAY;
    left_pb.value_style = &pb_lime_segmented_style;

    right_pb.top = 40;
    right_pb.left = 15;
    right_pb.width = 200;
    right_pb.max = 4095;
    right_pb.canvas = &right_pb_canvas;
    right_pb.background_color = TFT_DARKGREEN;
    right_pb.borders_thickness[0] = 1;
    right_pb.borders_thickness[1] = 1;
    right_pb.borders_thickness[2] = 1;
    right_pb.borders_thickness[3] = 1;  

    gui_led_init(main_led);
    gui_led_init(second_led);
    gui_pb_init(left_pb);
    gui_pb_init(right_pb);

    gui_led_begin(main_led);
    gui_led_begin(second_led);
    gui_pb_begin(left_pb);
    gui_pb_begin(right_pb);
}

void gui_analogread_task(void *arg)  
{
    while (1) 
    {
        auto left = analogRead(12);
        auto right = analogRead(13);

        //left_pb.value = left;
        right_pb.value = right;

        main_led.value = left > 2000;
        second_led.value = right > 500;

        gui_pb_update(left_pb);
        gui_pb_update(right_pb);

        gui_led_update(main_led);
        gui_led_update(second_led);

        vTaskDelay(100 / portTICK_RATE_MS);
    }
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
    //tft.setFreeFont(&NotoSansBold15);
    tft.loadFont(NotoSansBold15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(15, 80);

    String test = "Value:";
    tft.printf("Value: %d, %d", tft.fontHeight(), tft.textWidth(test));

    gui_init();

    envelope_init(envelope_context, I2S_SAMPLE_RATE);

    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);

    // xTaskCreate(mic_record_task, "mic_record_task", 2048, NULL, 1, NULL);

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
        int16_t samples_read = bytes_read / 2; //8
        
        if (samples_read > 0) 
        {
            float mean = 0;

            for (int16_t i = 0; i < samples_read; ++i) 
            {
                mean += (samples[i]);
            }
    
            // Average the data reading
            mean /= samples_read;
        
            envelope_calculate(samples, samples_read, envelope_context);

            Serial.print(envelope_context.envelope_out);

            Serial.print(" ");

            Serial.println(mean);

            left_pb.value = envelope_context.envelope_out;
        }  
    }
    // printf("loop cycling\n");
    // vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half of the cpu cycles
}