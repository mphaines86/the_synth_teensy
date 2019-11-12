//
// Created by michael on 11/9/19.
//

#include <stdint.h>
#include <system/HD44780.h>
#include "interface_parameters.h"
#include "interface.h"

void interfaceParameterHandleUserInput(int8_t input, uint16_t pot_value, uint8_t parameterNumber,
                                       interfaceParamType_e type) {
    static char dv[4] = {0};
    switch (type) {
        case parameterNone:
            return;
        case parameterCont:
            cpParameterList[parameterNumber] = pot_value;
            sprintf(dv, "%3d", cpParameterList[parameterNumber] >> 8);
            break;
        case parameterStep: {
            uint8_t v = pot_value >> (16 - spParameterBits[parameterNumber]);
            spParameterList[parameterNumber] = v;
            sprintf(dv, "%3d", spParameterList[parameterNumber]);
            break;
        }
        default:
            break;
    }
    if (input < NUM_OF_POTS / 2) {
        cposition(input * 5 + 1, 1);
    } else {
        cposition((input - 4) * 5 + 1, 3);
    }
    putsLCD(dv);
}

void interfaceParameterUpdatePage(uint8_t paramPage) {

}

