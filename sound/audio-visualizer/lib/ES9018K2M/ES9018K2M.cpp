#include <Wire.h>

#include "ES9018K2M.h"

const static uint8_t dac_address = 0x48;

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

void ES9018K2M_set_volume(uint8_t volume)
{
    writeI2c(dac_address, 0x0F, 0xFF - volume);
    writeI2c(dac_address, 0x10, 0xFF - volume);
}

uint8_t ES9018K2M_get_volume() {
    uint8_t res = 0;
    readI2c(dac_address, 0xFF, res);
    return res;
}

void ES9018K2M_init()
{
  Wire.begin(GPIO_NUM_21, GPIO_NUM_22);

  return;

  uint8_t res = 0; 

  if (readI2c(dac_address, 0x40, res)){
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

  //  Serial.println(DPLLNum);
  
  writeI2c(dac_address, 0x01, 0x0C);

  if (readI2c(dac_address, 0x01, res)){
    Serial.print("0x01 = ");
    Serial.println(res);
  }

   volumeDac(200);
}