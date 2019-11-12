//
// Created by michael on 11/9/19.
//

#ifndef THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H
#define THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H


#include "interface.h"

void interfaceParameterHandleUserInput(int8_t input, uint16_t pot_value, uint8_t parameterNumber,
                                       interfaceParamType_e type);
void interfaceParameterUpdatePage(uint8_t paramPage);
#endif //THE_SYNTH_TEENSY_INTERFACE_PARAMETERS_H
