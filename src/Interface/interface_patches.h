//
// Created by michael on 11/9/19.
//

#ifndef THE_SYNTH_TEENSY_INTERFACE_PATCHES_H
#define THE_SYNTH_TEENSY_INTERFACE_PATCHES_H

#include <stdint.h>
#include "system/EEPROM.h"

#define EEPROM_PATCH_CHECK 43
#define EEPROM_PATCH_DATA_SIZE EEPROM_DATA_BUFFER * 3
#define EEPROM_NUM_OF_PATCHES (EEPROM_SIZE - EEPROM_RESERVED)/EEPROM_PATCH_DATA_SIZE
#define EEPROM_PATCH_NAME_LENGTH 8

struct patch_struct_t {
    void setWriteProtect(uint8_t writeProtect);

    char * name[EEPROM_PATCH_NAME_LENGTH];
    uint8_t number;
    uint8_t writeProtect;
};

extern patch_struct_t patchInfo;

int8_t interfacePatchesInitSystem();
int8_t interfacePatchesInitPatch(uint8_t patch);
int8_t interfacePatchesLoadPatch(uint8_t patch);
int8_t interfacePatchesSavePatch(uint8_t patch);
void interfacePatchesUpdatePage(uint8_t patch);
void interfacePatchesSetWriteProtect(uint8_t value);
int8_t interfacePatchesUpdateName(int8_t input);

#endif //THE_SYNTH_TEENSY_INTERFACE_PATCHES_H
