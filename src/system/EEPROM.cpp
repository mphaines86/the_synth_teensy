//
// Created by michael on 12/17/19.
//

#include "i2c_t3.h"
#include "util/delay.h"
#include "EEPROM.h"

//TODO: Maybe Implement COBS encoding?

void EEPROMinit(){

    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_33_34, I2C_PULLUP_EXT, 400000);
    Wire.setDefaultTimeout(250000);
}

int8_t readData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size){
    Wire.beginTransmission(i2c_addr);
    Wire.write((uint8_t)(addr >> 8));
    Wire.write((uint8_t)(addr & 0xFF));
    //Wire.write((uint16_t)(0));
    //Wire.write((uint16_t)(0));

    switch(Wire.endTransmission()){
        case 0:
            break;
        case 1:
            Serial.println("Data too long");
            return -1;
        case 2:
            Serial.println("recieved NACK on address");
            return -2;
        case 3:
            Serial.println("recieved NACK on data");
            return -3;
        default:
            return 0;
    };

    uint8_t data_needed = size;

    if (size==0){
        return 0;
    }
    else{
        Wire.requestFrom(i2c_addr, (size_t) size);
        while(Wire.available()){
            *data++ = static_cast<uint8_t>(Wire.read());
            --data_needed;
        }
        if (data_needed!=0){
            Serial.println("Data not fully received");
            return -1;
        }
        return 1;
    }

}
int8_t writeData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size){

    uint8_t * value = &data[0];

    uint16_t data_size = size;

    if(size==0){
        return 0;
    }
    else {
        while (data_size != 0) {
            Wire.beginTransmission(i2c_addr);
            Wire.write((uint8_t) (addr >> 8));
            Wire.write((uint8_t) (addr & 0xFF));

            uint16_t page_length = (data_size <= EEPROM_DATA_BUFFER) ? data_size : EEPROM_DATA_BUFFER;
            for(uint8_t i=0; i<page_length; i++){
                Wire.write(*value++);
            }
            addr += page_length;

            switch(Wire.endTransmission()) {
                case 0:
                    break;
                case 1:
                    Serial.println("Data too long");
                    return -1;
                case 2:
                    Serial.println("recieved NACK on address");
                    return -2;
                case 3:
                    Serial.println("recieved NACK on data");
                    return -3;
                default:
                    break;
            };
            _delay_us(10000);
            data_size -= page_length;
        }
        return 1;
    }
}