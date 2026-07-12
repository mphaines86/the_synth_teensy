//
// Created by michael on 11/9/19.
//

#ifndef THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H
#define THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H


#include "interface.h"

uint8_t interfaceParameterHandleUserInput(InterfaceEventData* event, uint8_t paramPage);
void interfaceParameterUpdatePage(uint8_t paramPage);
#endif //THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H
