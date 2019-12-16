//
// Created by michael on 11/9/19.
//
#include <system/HD44780.h>
#include <util/delay.h>
#include "interface_patches.h"
#include "interface.h"
#include "synth.h"
#include "i2c_t3.h"

#define EEPROM_PATCH_CHECK 42
#define EEPROM_PATCH_DATA_SIZE EEPROM_DATA_BUFFER * 2



struct patch_struct_t patchInfo = {
        .name={nullptr},
        .number=0,
        .writeProtect=0
};

uint8_t data_buffer[EEPROM_PATCH_DATA_SIZE] = {0};

static int8_t readData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size){
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
static int8_t writeData(uint8_t i2c_addr, uint16_t addr, uint8_t *data, uint16_t size){

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

int8_t interfacePatchesInitSystem(){
    Wire.begin(I2C_MASTER, 0x00, I2C_PINS_33_34, I2C_PULLUP_EXT, 400000);
    Wire.setDefaultTimeout(250000);

    uint8_t patchNumber = 0;

    if(readData(80, 0, &patchNumber, 1) != 1){
        Serial.println("Failed to get Patch Number");
        return -1;
    }


    if (patchNumber > EEPROM_NUM_OF_PATCHES){
        patchNumber = 0;
        if(writeData(80, 0, &patchNumber, 1) != 1){
            Serial.println("Failed to set new patch number");
            return -2;
        }
    }

    patchInfo.number = patchNumber;

    uint8_t patchValid = 0;
    uint16_t patchAddress = EEPROM_PATCH_DATA_SIZE * patchNumber + 1;

    if(readData(80, patchAddress, &patchValid, 1) != 1) {
        Serial.println("Failed to check patch validity");
        return -1;
    }

    if(patchAddress != EEPROM_PATCH_CHECK){
        Serial.print("initiating patch ");
        Serial.println(patchNumber);
        if(interfacePatchesInitPatch(patchNumber)!=1){
            return -1;
        }
    }

    return 1;

}

int8_t interfacePatchesInitPatch(uint8_t patch){

    set_envelopes();
    set_lfo();
    set_oscillators();

    const char *name= "InitPtch";

    uint8_t data_index=PATCH_NAME_LENGTH+2;

    for (uint16_t i : cpParameterList) {
        data_buffer[data_index] = (uint8_t) (i >> 8);
        data_index++;
        data_buffer[data_index] = (uint8_t) (i & 0xFF);
        data_index++;
    }

    for (uint16_t i : spParameterList) {
        data_buffer[data_index] = (uint8_t) (i >> 8);
        data_index++;
        data_buffer[data_index] = (uint8_t) (i & 0xFF);
        data_index++;
    }
    for(uint8_t i=data_index; i < EEPROM_PATCH_DATA_SIZE; i++){
        data_buffer[data_index] = 0;
        data_index++;
    }

    const char *value = &name[0];

    data_buffer[0] = EEPROM_PATCH_CHECK -1;
    for(uint8_t i=1; i<PATCH_NAME_LENGTH+1; i++){
        data_buffer[i] = (uint8_t)*value++;
    }

    data_buffer[PATCH_NAME_LENGTH+1] = 0;

    if(writeData(80, patch*EEPROM_PATCH_DATA_SIZE+1, data_buffer, EEPROM_PATCH_DATA_SIZE)!=1){
        return -1;
    }

    return 1;
}

int8_t interfacePatchesLoadPatch(uint8_t patch){

    uint8_t patchValid = 0;
    uint16_t patchAddress = EEPROM_PATCH_DATA_SIZE * patchInfo.number + 1;

    if(readData(80, patchAddress, &patchValid, 1) != 1) {
        return -1;
    }

    if(patchAddress != EEPROM_PATCH_CHECK){
        interfacePatchesInitPatch(patchInfo.number);
    }


    if(readData(80, patchAddress+1, data_buffer, EEPROM_PATCH_DATA_SIZE) != 1)
        return -1;

    memcpy(&patchInfo.name, &data_buffer, PATCH_NAME_LENGTH*sizeof(uint8_t));
    uint8_t data_index = 9;

    for (uint16_t &i : cpParameterList) {
        i = (data_buffer[data_index] << 8) | (data_buffer[data_index+1]);
        data_index+=2;
    }

    for (uint16_t &i : spParameterList) {
        i = (data_buffer[data_index] << 8) | (data_buffer[data_index+1]);
        data_index+=2;
    }

    if(writeData(80, 0, &patch, 1)!=1){
        return -1;
    }

    return 1;
}

int8_t interfacePatchesSavePatch(uint8_t patch){const char *name= "InitPtch";

    uint8_t data_index=PATCH_NAME_LENGTH+2;

    for (uint16_t i : cpParameterList) {
        data_buffer[data_index] = (uint8_t) (i >> 8);
        data_index++;
        data_buffer[data_index] = (uint8_t) (i & 0xFF);
        data_index++;
    }

    for (uint16_t i : spParameterList) {
        data_buffer[data_index] = (uint8_t) (i >> 8);
        data_index++;
        data_buffer[data_index] = (uint8_t) (i & 0xFF);
        data_index++;
    }
    for(uint8_t i=data_index; i < EEPROM_PATCH_DATA_SIZE; i++){
        data_buffer[data_index] = 0;
        data_index++;
    }

    data_buffer[0] = EEPROM_PATCH_CHECK;
    for(uint8_t i=1; i<PATCH_NAME_LENGTH+1; i++){
        data_buffer[i] = (uint8_t)*name++;
    }
    data_buffer[PATCH_NAME_LENGTH+1] = patchInfo.writeProtect;

    if(writeData(80, patch*EEPROM_PATCH_DATA_SIZE+1, data_buffer, EEPROM_PATCH_DATA_SIZE)!=1){
        return -1;
    }
    
    return 1;
}

void interfacePatchesUpdatePage(uint8_t patch) {
    patchInfo.number=patch;
    interfacePatchesLoadPatch(patch);

    cmd2LCD(0x01);
    delay(2);

    static char dv[4] = {0};
    cposition(0, 0);
    sprintf(dv, "%3d:", patchInfo.number);
    putsLCD(dv);
    putsLCD((char *) patchInfo.name);
}