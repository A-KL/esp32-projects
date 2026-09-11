#ifndef USER_CONFIG_H
#define USER_CONFIG_H

//spi & i2c handle
#define LCD_HOST SPI3_HOST

// touch I2C port
#define Touch_SCL_NUM (GPIO_NUM_18)
#define Touch_SDA_NUM (GPIO_NUM_17)

// touch esp
#define ESP_SCL_NUM (GPIO_NUM_48)
#define ESP_SDA_NUM (GPIO_NUM_47)

//  DISP
#define EXAMPLE_PIN_NUM_LCD_CS     (GPIO_NUM_9) 
#define EXAMPLE_PIN_NUM_LCD_PCLK   (GPIO_NUM_10)
#define EXAMPLE_PIN_NUM_LCD_DATA0  (GPIO_NUM_11)
#define EXAMPLE_PIN_NUM_LCD_DATA1  (GPIO_NUM_12)
#define EXAMPLE_PIN_NUM_LCD_DATA2  (GPIO_NUM_13)
#define EXAMPLE_PIN_NUM_LCD_DATA3  (GPIO_NUM_14)
#define EXAMPLE_PIN_NUM_LCD_RST    (GPIO_NUM_21)
#define EXAMPLE_PIN_NUM_BK_LIGHT   (GPIO_NUM_42) 


#define TOUCH_I2C_ADDR        0x3b
#define TOUCH_RST_NUM         (-1)
#define TOUCH_INT_NUM         (-1)

/*ADDR*/
#define EXAMPLE_RTC_ADDR 0x51
#define EXAMPLE_IMU_ADDR 0x6b


#define USER_DISP_ROT_90    1
#define USER_DISP_ROT_NONO  0
#define Rotated USER_DISP_ROT_90


#if (Rotated == USER_DISP_ROT_NONO)
#define EXAMPLE_LCD_H_RES 172   
#define EXAMPLE_LCD_V_RES 640
#else
#define EXAMPLE_LCD_H_RES 640   
#define EXAMPLE_LCD_V_RES 172
#endif

#define LCD_NOROT_HRES     172
#define LCD_NOROT_VRES     640
#define LVGL_DMA_BUFF_LEN (LCD_NOROT_HRES * 64 * 2)
#define LVGL_SPIRAM_BUFF_LEN (EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES * 2)

#endif