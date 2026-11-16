#include <Wire.h>

#include "ES9018K2M.h"

#ifndef I2C_DEV
# define I2C_DEV Wire
#endif

#define ES9018K2M_I2C_ADDRESS  0x48

#define ES9018K2M_CHIP_STATUS  0x40
#define ES9018K2M_GENERAL      0x07  /* General Settings */
#define ES9018K2M_VOLUME1      0x0F  /* Usually selected for the Left Channel */
#define ES9018K2M_VOLUME2      0x10  /* Usually selected for the Right Channel */
#define ES9018K2M_INPUT_CONFIG 0x01 /* Input Configuration */

struct es9018_handle_t {
  const uint8_t chip_id = 0x00;
  uint8_t address = ES9018K2M_I2C_ADDRESS;
  uint8_t general_settings = 0x80;
  uint8_t volume_left = 0x00;
  uint8_t volume_right = 0x00;
  uint8_t input_config = 0x8C;
};

bool i2c_read(TwoWire& i2c, const uint8_t address, const uint8_t reg, uint8_t& value) 
{
  i2c.beginTransmission(address);
  i2c.write(reg);

  auto error = i2c.endTransmission();

  if (error) {
    log_e("I2C error %d reg %d code: %d", address, reg, error);    
    return false;
  }
  auto to_receive = sizeof(value);
  error = i2c.requestFrom(address, to_receive);

   if (i2c.available() <= to_receive) { 
    value = i2c.read();  
  }

  return true;
}

bool i2c_write(TwoWire& i2c, uint8_t address, uint8_t reg, uint8_t value) {
  i2c.beginTransmission(address);
  i2c.write(reg);
  i2c.write(value);
  auto error = i2c.endTransmission();

  if (error) {
    log_e("I2C error %d reg %d code: %d", address, reg, error);    
    return false;
  }

  return true;
}

bool ES9018K2M_write_general_settings(const es9018_handle_t& handle)
{
  return i2c_write(I2C_DEV, handle.address, ES9018K2M_GENERAL, handle.general_settings);
}

bool ES9018K2M_set_mute(es9018_handle_t& handle, const bool mute)
{
  if (mute) {
    handle.general_settings |= 0x01;
  } else {
    handle.general_settings &= 0xFE;
  }

  return i2c_write(I2C_DEV, handle.address, ES9018K2M_GENERAL, handle.general_settings);
}

// 0dB to –127.5dB in 0.5dB steps
bool ES9018K2M_set_volume(es9018_handle_t& handle, const uint8_t volume)
{
  handle.volume_left = 0xFF - volume;
  handle.volume_right = 0xFF - volume;

  return i2c_write(I2C_DEV, handle.address, ES9018K2M_VOLUME1, handle.volume_left) &&
         i2c_write(I2C_DEV, handle.address, ES9018K2M_VOLUME2, handle.volume_right);
}

uint8_t ES9018K2M_get_volume(es9018_handle_t& handle) 
{
    uint8_t res = 0;
    i2c_read(I2C_DEV, handle.address, 0xFF, res);
    return res;
}

bool ES9018K2M_init(es9018_handle_t& handle)
{
 // Wire.begin(GPIO_NUM_21, GPIO_NUM_22);

  uint8_t res = 0;

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_CHIP_STATUS, res)) {
    return false;
  }
  log_e("0x40 = %d", res);

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_GENERAL, handle.general_settings)) {
    return false;
  }
  log_e("Reg: %d = %d\n", ES9018K2M_GENERAL, handle.general_settings);

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_VOLUME1, handle.volume_left)) {
    return false;
  }
  log_e("Volume 1: %d = %d\n", ES9018K2M_VOLUME1, handle.volume_left);

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_VOLUME2, handle.volume_right)) {
    return false;
  }
  log_e("Volume 2: %d = %d\n", ES9018K2M_VOLUME2, handle.volume_right);

  // writeI2c(0x48, 0x0A, 0x05 | (1<<5) | (1<<6) | (1<<0));
  // writeI2c(0x48, 0x06, 0x4A | (1<<4));

  // unsigned long DPLLNum=0;
  // for (auto i=0; i<4; i++){
  //   if (i2c_read(0x48, 31 - i, res)){
  //     Serial.print(31 - i);
  //     Serial.print(" = ");
  //     Serial.println(res);

  //     DPLLNum|=res;

  //     if(i != 3) {
  //       DPLLNum<<=8;
  //     }
  //   }
  // }

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_INPUT_CONFIG, res)){
    return false;
  }
  log_e("Reg: %d = %d\n", ES9018K2M_INPUT_CONFIG, res);
  
  // 16bit
  if (!i2c_write(I2C_DEV, handle.address, ES9018K2M_INPUT_CONFIG, 0x0C)) {
    return false;
  }

  if (!i2c_read(I2C_DEV, handle.address, ES9018K2M_INPUT_CONFIG, res)){
    return false;
  }
  log_e("Reg: %d = %d\n", ES9018K2M_INPUT_CONFIG, res);

// volumeDac(200);
  return true;
}