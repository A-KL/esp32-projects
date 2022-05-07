#include <Wire.h>

bool readI2c(uint8_t address, uint8_t reg, uint8_t& value) {
  Wire.beginTransmission(address);
  Wire.write(byte(reg));
  auto error = Wire.endTransmission();

  if (error) {
    Serial.print("I2C error ");
    Serial.print(address);
    Serial.print(" reg ");
    Serial.print(reg);
    Serial.print(" code: ");
    Serial.print(error);
    
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
    Serial.print("I2C error ");
    Serial.print(address);
    Serial.print(" reg ");
    Serial.print(reg);
    Serial.print(" code: ");
    Serial.print(error);
    
    return false;
  }

  return true;
}

void volumeDac(uint8_t volume)
{
    writeI2c(0x48, 0x0F, 0xFF - volume);
    writeI2c(0x48, 0x10, 0xFF - volume);
}

void initDac()
{
  Wire.begin(GPIO_NUM_21, GPIO_NUM_22);

  uint8_t res = 0; 

  if (readI2c(0x48, 0x40, res)){
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
  
  writeI2c(0x48, 0x01, 0x0C);

  if (readI2c(0x48, 0x01, res)){
    Serial.print("0x01 = ");
    Serial.println(res);
  }

   volumeDac(200);
}