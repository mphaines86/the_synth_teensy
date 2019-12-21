//
// Created by michael on 12/17/19.
//

#ifndef THE_SYNTH_TEENSY_EEPROM_H
#define THE_SYNTH_TEENSY_EEPROM_H

#include <cstdint>

#define EEPROM_SIZE 32000
#define EEPROM_RESERVED 3200
#define EEPROM_DATA_BUFFER 64

void EEPROMinit();
int8_t readData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size);
int8_t writeData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size);

#endif //THE_SYNTH_TEENSY_EEPROM_H
