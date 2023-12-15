#include <M5StickCPlus.h>
/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/m5stickc
*
* Describe: Micophone.
* Date: 2022/2/22
*******************************************************************************
*/
//#include <M5StickC.h>
#include <driver/i2s.h>

#define PIN_CLK     0
#define PIN_DATA    34
#define READ_LEN    (2 * 256)
#define GAIN_FACTOR 30
#define SAMPLE_RATE 44100
#define LCD_W 240
#define LCD_H 135

#define BAR_W 20
#define BAR_H 135

uint8_t bar_value = 0;

uint8_t BUFFER[READ_LEN] = {0};
uint16_t oldy[160];
int16_t *adcBuffer = NULL;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX |
                          I2S_MODE_PDM),  // Set the I2S operating mode.
    .sample_rate = SAMPLE_RATE,           // Set the I2S sampling rate.
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,  // Set the channel format.

#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
        I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.
    .dma_buf_count = 2,        // DMA buffer count.
    .dma_buf_len   = 128,      // DMA buffer length.
};

void i2sInit()  // Init I2S.  初始化I2S
{
    i2s_pin_config_t pin_config;

#if (ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0))
    pin_config.mck_io_num = I2S_PIN_NO_CHANGE;
#endif

    pin_config.bck_io_num   = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num    = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num  = PIN_DATA;
    pin_config.mck_io_num   = GPIO_NUM_0;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

float EnvelopeOut;
float envIn;
float attack = 1; //0.996879878
float release = 1;// 0.9996879878;
float DC_Filter = 0;
float X_Prev = 0;
float X= 0;

void showSignal() {
    int y;

    auto value =  map(EnvelopeOut, 0, INT16_MAX, 0, BAR_H);
    auto bx = LCD_W - BAR_W;
    auto by = LCD_H - value;

    //Serial.println(value);

    M5.Lcd.fillRect(bx, 0, BAR_W, BAR_H, WHITE);
    M5.Lcd.fillRect(bx, by, BAR_W, value, BLACK);

    for (int n = 0; n < 160; n++) {
        y = adcBuffer[n] * GAIN_FACTOR;
        y = map(y, INT16_MIN, INT16_MAX, 10, 70);

        M5.Lcd.drawPixel(n, oldy[n], WHITE);
        M5.Lcd.drawPixel(n, y, BLACK);

        oldy[n] = y;
    }
}

void mic_record_task(void *arg)  {
    size_t bytesread;

    while (1) {
        i2s_read(I2S_NUM_0, (char *)BUFFER, READ_LEN, &bytesread, (100 / portTICK_RATE_MS));

        adcBuffer = (int16_t *)BUFFER;

        for (auto i=0; i<bytesread/2; i++) {
            X = adcBuffer[i];

            X = X * 300; //300;

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
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(WHITE);
    M5.Lcd.setTextColor(BLACK, WHITE);
    M5.Lcd.println("mic test");

    attack = expf(-1.0/((float)SAMPLE_RATE * .050)); //50mS Attack
    release = expf(-1.0/((float)SAMPLE_RATE * .100)); //100mS Release

    i2sInit();
    xTaskCreate(mic_record_task, "mic_record_task", 2048, NULL, 1, NULL);
}

/* 
After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
*/
void loop() {
    printf("loop cycling\n");
    vTaskDelay(1000 / portTICK_RATE_MS);  // otherwise the main task wastes half
                                          // of the cpu cycles
}