#ifndef CommUtil_h
#define CommUtil_h

#include <Arduino.h>
#include <Wire.h>

class CommUtil {
   public:
    CommUtil();
    bool writeCommand(uint8_t address, uint8_t subAddress);
    bool writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
    bool writeBytes(uint8_t address, uint8_t subAddress, uint8_t *data,
                    uint8_t length);
    bool readByte(uint8_t address, uint8_t *result);
    bool readByte(uint8_t address, uint8_t subAddress, uint8_t *result);
    bool readBytes(uint8_t address, uint8_t count, uint8_t *dest);
    bool readBytes(uint8_t address, uint8_t subAddress, uint8_t count,
                   uint8_t *dest);
    void scanID(bool *result);

   private:
};
#endif