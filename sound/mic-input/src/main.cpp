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
#define GAIN_FACTOR 3 //30?

uint8_t BUFFER[READ_LEN] = {0};

uint16_t oldy[160];
int16_t *adcBuffer = NULL;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX |
                          I2S_MODE_PDM),  // Set the I2S operating mode.
    .sample_rate = 44100,  // Set the I2S sampling rate.
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,  // Fixed 12-bit stereo MSB.
    .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,  // Set the channel format.

#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 1, 0)
    .communication_format =
        I2S_COMM_FORMAT_STAND_I2S,  // Set the format of the communication.
#else                                   // 设置通讯格式
    .communication_format = I2S_COMM_FORMAT_I2S,
#endif
    .intr_alloc_flags =
        ESP_INTR_FLAG_LEVEL1,  // Set the interrupt flag.  设置中断的标志
    .dma_buf_count = 2,        // DMA buffer count.  DMA缓冲区计数
    .dma_buf_len   = 128,      // DMA buffer length.  DMA缓冲区长度
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

    //pin_config.mck_io_num = GPIO_NUM_0;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void showSignal() {
    int y;
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
        i2s_read(I2S_NUM_0, (char *)BUFFER, READ_LEN, &bytesread,
                 (100 / portTICK_RATE_MS));

        adcBuffer = (int16_t *)BUFFER;

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