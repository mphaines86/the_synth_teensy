//
// Created by michael on 11/9/19.
//

#ifndef THE_SYNTH_TEENSY_INTERFACE_SAMPLE_H
#define THE_SYNTH_TEENSY_INTERFACE_SAMPLE_H

uint8_t interfaceSampleFindZeroPoint(uint8_t page, uint8_t paramPage);
void interfaceSampleHandleUserInput(uint8_t input, uint8_t paramPage, uint16_t *pot_value);
void interfaceSampleUpdatePage(uint8_t paramPage);
#endif //THE_SYNTH_TEENSY_INTERFACE_SAMPLE_H
