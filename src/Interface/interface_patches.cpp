//
// Created by michael on 11/9/19.
//
#include <system/HD44780.h>
#include <util/delay.h>
#include "interface_patches.h"
#include "interface.h"
#include "synth.h"
#include "main.h" //TODO: Remove and improve DEBUG code

struct patch_struct_t patchInfo = {
        .name={nullptr},
        .number=0,
        .writeProtect=0
};

static uint8_t data_buffer[EEPROM_PATCH_DATA_SIZE] = {0};
uint8_t patch_name_edit_marker = 0;



int8_t interfacePatchesInitSystem(){

    uint8_t patchNumber = 0;

    if(readData(80, 0, &patchNumber, 1) != 1){
#ifdef DEBUG
        Serial.println("PatchInitSys: Failed to get Patch Number");
#endif
        return -1;
    }


    if (patchNumber > EEPROM_NUM_OF_PATCHES){
        patchNumber = 0;
        if(writeData(80, 0, &patchNumber, 1) != 1){
#ifdef DEBUG
            Serial.println("PatchInitSys: Failed to set new patch number");
#endif
            return -2;
        }
    }

    patchInfo.number = patchNumber;

    uint8_t patchValid = 0;
    uint16_t patchAddress = EEPROM_PATCH_DATA_SIZE * patchNumber + 1;

    if(readData(80, patchAddress, &patchValid, 1) != 1) {
#ifdef DEBUG
        Serial.println("PatchInitSys: Failed to check patch validity");
#endif
        return -1;
    }

    if(patchValid != EEPROM_PATCH_CHECK){

        if(interfacePatchesInitPatch(patchNumber)!=1){
#ifdef DEBUG
            Serial.print("PatchInitSys: Failed to init patch");
            Serial.println(patchNumber);
#endif
            return -1;
        }
    }

    if(interfacePatchesLoadPatch(patchInfo.number)!=1){
#ifdef DEBUG
        Serial.println("PatchInitSys: Failed to load patch");
#endif
        return -1;
    }

    return 1;

}

int8_t interfacePatchesInitPatch(uint8_t patch){

    set_envelopes();
    set_lfo();
    set_oscillators();

    const char *name= "InitPtch";

    uint8_t data_index=EEPROM_PATCH_NAME_LENGTH+2;

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

    data_buffer[0] = EEPROM_PATCH_CHECK;
    for(uint8_t i=1; i<EEPROM_PATCH_NAME_LENGTH+1; i++){
        data_buffer[i] = (uint8_t)*value++;
    }

    data_buffer[EEPROM_PATCH_NAME_LENGTH+1] = 0;

    if(writeData(80, patch*EEPROM_PATCH_DATA_SIZE+1, data_buffer, EEPROM_PATCH_DATA_SIZE)!=1){
        return -1;
    }

    memcpy(&patchInfo.name[0], &name[0], EEPROM_PATCH_NAME_LENGTH*sizeof(char));

    return 1;
}

int8_t interfacePatchesLoadPatch(uint8_t patch){

    uint8_t patchValid = 0;
    uint16_t patchAddress = EEPROM_PATCH_DATA_SIZE * patchInfo.number + 1;

    if(readData(80, patchAddress, &patchValid, 1) != 1) {
#ifdef DEBUG
        Serial.println("PatchLoad: Failed to Verify patch");
#endif
        return -1;
    }

    if(patchValid != EEPROM_PATCH_CHECK){
#ifdef DEBUG
        Serial.println("PatchLoad: Patch is not valid");
#endif
        interfacePatchesInitPatch(patchInfo.number);
    }


    if(readData(80, patchAddress+1, data_buffer, EEPROM_PATCH_DATA_SIZE) != 1)
        return -1;

    memcpy(&patchInfo.name[0], (char *) &data_buffer[0], EEPROM_PATCH_NAME_LENGTH*sizeof(char));
    uint8_t data_index = 9;

    for (uint16_t &i : cpParameterList) {
        i = (uint16_t) (data_buffer[data_index] << 8) | (data_buffer[data_index+1]);
        data_index+=2;
    }
    for (uint16_t &i : spParameterList) {
        i = (uint16_t) (data_buffer[data_index] << 8) | (data_buffer[data_index+1]);
        data_index+=2;
    }

    if(writeData(80, 0, &patch, 1)!=1){
        return -1;
    }
#ifdef DEBUG
    Serial.print("PatchLoad: Loaded Patch ");
    Serial.println(patch);
#endif

    synthParameterChange();

    return 1;
}

int8_t interfacePatchesSavePatch(uint8_t patch){
    if(patchInfo.writeProtect){
        lcdCmd(0x01);
        delay(2);
        lcdChangePos(0, 0);
        const char *row1 = "Patch Write Protected!";
        lcdSendCharArray(row1);
        delay(3000);

        return 0;
    }

    const char *name= "InitPtch";

    uint8_t data_index=EEPROM_PATCH_NAME_LENGTH+2;

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

    const char *value = patchInfo.name[0];
    memcpy(&data_buffer[1], (uint8_t *) &patchInfo.name[0], EEPROM_PATCH_NAME_LENGTH*sizeof(uint8_t));
    data_buffer[0] = EEPROM_PATCH_CHECK;
    //for(uint8_t i=1; i<EEPROM_PATCH_NAME_LENGTH+1; i++){
        //memcpy(&data_buffer[i], &patchInfo.name[i-1], EEPROM_PATCH_NAME_LENGTH*sizeof(uint8_t));
        //data_buffer[i] = (uint8_t) *(*(patchInfo.name+i));
    //}
    data_buffer[EEPROM_PATCH_NAME_LENGTH+1] = patchInfo.writeProtect;

    if(writeData(80, patch*EEPROM_PATCH_DATA_SIZE+1, data_buffer, EEPROM_PATCH_DATA_SIZE)!=1){
#ifdef DEBUG
        Serial.println("PatchSave: Failed to save patch");
#endif
        return -1;
    }

#ifdef DEBUG
    Serial.print("PatchSave: Saved Patch ");
    Serial.println(patch);
#endif

    return 1;
}

void interfacePatchesUpdatePage(uint8_t patch) {
    if(patch!=patchInfo.number) {
        patchInfo.number = patch;
        interfacePatchesLoadPatch(patch);
    }

    lcdCmd(0x01);
    delay(2);

#ifdef DEBUG
    Serial.println((char *) &patchInfo.name[0]);
#endif

    static char dv[4] = {0};
    lcdChangePos(0, 0);
    sprintf(dv, "%3d:", patchInfo.number);
    lcdSendCharArray(dv);
    lcdSendCharArray((char *) patchInfo.name);
}

void interfacePatchesSetWriteProtect(uint8_t value){

    lcdCmd(0x01);
    delay(2);
    lcdChangePos(0, 0);
    const char *row1 = "Write Protect Set";
    lcdSendCharArray(row1);

    patchInfo.writeProtect = value;
    writeData(80, patchInfo.number*EEPROM_PATCH_DATA_SIZE + EEPROM_PATCH_NAME_LENGTH + 1, &patchInfo.writeProtect, 1);

    delay(3000);

}

int8_t interfacePatchesUpdateName(int8_t input){

    unsigned char character;

    if (input == 10){
        lcdCmd(0x01);
        delay(2);
        lcdChangePos(0, 0);
        const char *row1 = "Patch Name:";
        lcdSendCharArray(row1);
        for(uint8_t i=0; i<EEPROM_PATCH_NAME_LENGTH; i++) {
            character = static_cast<unsigned char>(96 * (interfaceGetPotValues(i) >> 8) / 255 + 32);
            lcdChangePos(i, 1);
            lcdSendChar(character);
            //patchInfo.name[i] = reinterpret_cast<char *>(character);
            //*(patchInfo.name + i) = (char *) character;
        }
        //patch_name_edit_marker++;
    }

    if (input==2){
        unsigned char buffer[EEPROM_PATCH_NAME_LENGTH] = {0};
        for(uint8_t i=0; i < EEPROM_PATCH_NAME_LENGTH; i++){
            buffer[i] = static_cast<unsigned char>(96 * (interfaceGetPotValues(i) >> 8) / 255 + 32);
        }
        memcpy(&patchInfo.name[0], (char *) &buffer[0], EEPROM_PATCH_NAME_LENGTH*sizeof(char));
        //patch_name_edit_marker=0;
        return 1;
    }
    else if (input > 2 && input < 10){
        return 1;
    }
    input = -input - 1;
    character = static_cast<unsigned char>(96 * (interfaceGetPotValues(input) >> 8) / 255 + 32);
    //patchInfo.name[input] = reinterpret_cast<char *>(character);

    lcdChangePos(input, 1);
    lcdSendChar(character);
    //Serial.println(character);
    return 0;

}