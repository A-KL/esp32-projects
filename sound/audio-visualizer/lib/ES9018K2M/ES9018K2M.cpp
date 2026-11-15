#include <Wire.h>

#include "ES9018K2M.h"

#define ES9018K2M_I2C_ADDRESS  0x48

#define ES9018K2M_GENERAL      0x07  /* General Settings */
#define ES9018K2M_VOLUME1      0x0F  /* Usually selected for the Left Channel */
#define ES9018K2M_VOLUME2      0x10  /* Usually selected for the Right Channel */
#define ES9018K2M_CHIP_STATUS  0x40

#define ES9018K2M_INPUT_CONFIG 0x01 /* Input Configuration */

struct es9018_handle_t {
  uint8_t general_settings = 0x80;
  uint8_t volume_left = 0x00;
  uint8_t volume_right = 0x00;
  uint8_t input_config = 0x8C;
  const uint8_t chip_id = 0x00;
}

bool readI2c(uint8_t address, uint8_t reg, uint8_t& value) 
{
  Wire.beginTransmission(address);
  Wire.write(byte(reg));

  auto error = Wire.endTransmission();

  if (error) {
    log_e("I2C error %d reg %d code: %d", address, reg, error);    
    return false;
  }
  auto to_receive = sizeof(value);
  error = Wire.requestFrom(address, to_receive);

   if (Wire.available() <= to_receive) { 
    value = Wire.read();  
  }

  return true;
}

bool writeI2c(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(byte(reg));
  Wire.write(byte(value));
  auto error = Wire.endTransmission();

  if (error) {
    log_e("I2C error %d reg %d code: %d", address, reg, error);    
    return false;
  }

  return true;
}

bool void ES9018K2M_write_general_settings(const es9018_handle_t& handle)
{
  return writeI2c(ES9018K2M_I2C_ADDRESS, ES9018K2M_GENERAL, handle.general_settings);
}

bool ES9018K2M_set_mute(es9018_handle_t& handle, bool mute)
{
  if (mute) {
    handle.general_settings |= 0x01;
  } else {
    handle.general_settings &= 0xFE;
  }

  return writeI2c(ES9018K2M_I2C_ADDRESS, ES9018K2M_GENERAL, dac_general_settings)
}

// 0dB to –127.5dB in 0.5dB steps

void ES9018K2M_set_volume(uint8_t volume)
{
    writeI2c(dac_address, ES9018K2M_VOLUME1, 0xFF - volume);
    writeI2c(dac_address, ES9018K2M_VOLUME2, 0xFF - volume);
}

uint8_t ES9018K2M_get_volume() {
    uint8_t res = 0;
    readI2c(dac_address, 0xFF, res);
    return res;
}

bool ES9018K2M_init()
{
 // Wire.begin(GPIO_NUM_21, GPIO_NUM_22);

  uint8_t res = 0; 

  if (readI2c(ES9018K2M_I2C_ADDRESS, ES9018K2M_CHIP_STATUS, res)) {
    Serial.print("0x40 = ");
    Serial.println(res);
  }

  // writeI2c(0x48, 0x0A, 0x05 | (1<<5) | (1<<6) | (1<<0));
  // writeI2c(0x48, 0x06, 0x4A | (1<<4));

  // unsigned long DPLLNum=0;
  // for (auto i=0; i<4; i++){
  //   if (readI2c(0x48, 31 - i, res)){
  //     Serial.print(31 - i);
  //     Serial.print(" = ");
  //     Serial.println(res);

  //     DPLLNum|=res;

  //     if(i != 3) {
  //       DPLLNum<<=8;
  //     }
  //   }
  // }
  
  auto r = writeI2c(ES9018K2M_I2C_ADDRESS, ES9018K2M_INPUT_CONFIG, 0x0C);

  if (readI2c(ES9018K2M_I2C_ADDRESS, ES9018K2M_INPUT_CONFIG, res)){
    Serial.printf("Reg: %d = %d\n", ES9018K2M_INPUT_CONFIG, res);
  }

  return r;
  //  volumeDac(200);
}