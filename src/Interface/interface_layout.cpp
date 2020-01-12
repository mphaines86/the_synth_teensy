//
// Created by michael on 11/9/19.
//

#include <cstdint>
#include <system/EEPROM.h>
//#include <usb_serial.h>
#include <Osc.h>
#include <synth.h>
#include "interface_layout.h"
#include "interface_patches.h"
#include "interface.h"

#define NUMBER_OF_ZONES 8
#define LAYOUT_DATA_LENGTH 2*NUMBER_OF_ZONES*NUMBER_OF_OSCILLATORS + 2

static struct {
    uint8_t zone_limits[NUMBER_OF_OSCILLATORS][NUMBER_OF_ZONES+2];
    uint8_t zone_wave[NUMBER_OF_OSCILLATORS][NUMBER_OF_ZONES];
    uint8_t enabled_zones[NUMBER_OF_OSCILLATORS];
} zone_struct;

static uint8_t data_buffer[EEPROM_PATCH_DATA_SIZE] = {0};

int8_t interfaceLayoutInit(uint8_t patch) {
    for(uint8_t oscillator=0; oscillator<NUMBER_OF_OSCILLATORS; oscillator++){
        for(uint8_t zone=0; zone<NUMBER_OF_ZONES; zone++) {
            zone_struct.zone_limits[oscillator][zone] = 0;
            zone_struct.zone_wave[oscillator][zone] = 0;
        }
        zone_struct.enabled_zones[oscillator] = 1;
        zone_struct.zone_limits[oscillator][1] = 127;
    }

    if(interfaceLayoutSave(patch)!=1){
        return -1;
    }

    return 1;
}

int8_t interfaceLayoutLoad(uint8_t patch) {
    uint16_t data_location = patch*EEPROM_PATCH_DATA_SIZE + 1 + EEPROM_PATCH_NAME_LENGTH + spCount + cpCount + 1;
    if(readData(80, data_location, data_buffer, LAYOUT_DATA_LENGTH)!=1){
        Serial.println("Failed to get layout Information");
        return -1;
    };

    uint8_t data_element = 0;

    for(uint8_t oscillator=0; oscillator<NUMBER_OF_OSCILLATORS;oscillator++) {
        zone_struct.enabled_zones[oscillator] = data_buffer[data_element++];

        for (uint8_t zone = 0; data_element < NUMBER_OF_ZONES + 2; zone++) {
            zone_struct.zone_limits[oscillator][zone] = data_buffer[data_element++];
        }

        for (uint8_t zone=0; zone < NUMBER_OF_ZONES; ++zone) {
            zone_struct.zone_wave[oscillator][zone] = data_buffer[data_element++];
        }

        for(uint8_t zone=1; zone< zone_struct.enabled_zones[oscillator]; zone++){
            for(uint8_t voice=0; voice < SYNTH_VOICE_COUNT; voice++) {
                synthWaveChange(zone_struct.zone_wave[oscillator][zone], voice, oscillator,
                        zone_struct.zone_limits[oscillator][zone], zone_struct.zone_limits[oscillator][zone+1]);
            }
        }
    }

    return 1;

}

int8_t interfaceLayoutSave(uint8_t patch) {

    uint8_t data_element = 0;

    for(uint8_t oscillator=0; oscillator<NUMBER_OF_OSCILLATORS; oscillator++){
        data_buffer[data_element++] = zone_struct.enabled_zones[oscillator];

        for (uint8_t zone = 0; zone < NUMBER_OF_ZONES + 2; zone++){
            data_buffer[data_element++] = zone_struct.zone_limits[oscillator][zone];
        }
        for (uint8_t zone = 0; zone < NUMBER_OF_ZONES; zone++){
            data_buffer[data_element++] = zone_struct.zone_wave[oscillator][zone];
        }

    }
    uint16_t data_location = patch*EEPROM_PATCH_DATA_SIZE + 1 + EEPROM_PATCH_NAME_LENGTH + spCount + cpCount + 1;

    if(writeData(80, data_location, data_buffer, data_element)!=1){
        Serial.println("Failed to save layout information");
        return -1;
    }

    return 1;

}

void interfaceLayoutUpdatePage(uint8_t paramPage) {
    if (paramPage == 0){

    }
}

void interfaceLayoutHandleUserInput() {

}



