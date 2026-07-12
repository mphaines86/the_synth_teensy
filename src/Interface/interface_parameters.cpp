//
// Created by michael on 11/9/19.
//

#include <stdint.h>
#include <system/HD44780.h>
#include "interface_parameters.h"
#include "interface.h"
#include "interface_patches.h"
#include "../parameter.h"


uint8_t interfaceParameterHandleUserInput(InterfaceEventData* event, uint8_t paramPage) {
    // Make entire process a single switch statement
    uint8_t input = event->type;
    uint16_t pot_value = event->data.pot.value;
    int8_t pot_number = event->data.pot.pot_id;
    //Serial.print("Update Parameters: ");
    //Serial.print(input);
    //Serial.print(", ");
    //Serial.println(pot_value);
    switch (input){
        case EVENT_BUTTON_TWO_PRESSED:{
            interfacePatchesSavePatch(patchInfo.number);

            lcdClearScreen();
            lcdChangePos(0,0);
            interfacePatchesSavePatch(patchInfo.number);
            lcdSendCharArray((char *) "Saving Patch");
            delay(3000);

            interfaceParameterUpdatePage(paramPage);
            return 0;
        }
        case EVENT_BUTTON_THREE_PRESSED:{
            //interfacePatchesSetWriteProtect(static_cast<uint8_t>(!patchInfo.writeProtect));

            lcdClearScreen();
            lcdChangePos(0,0);
            interfacePatchesInitPatch(patchInfo.number);
            lcdSendCharArray((char *) "Initializing Patch");
            delay(3000);

            interfaceParameterUpdatePage(paramPage);
            return 0;
        }
        case EVENT_BUTTON_FOUR_PRESSED:{
            interfacePatchesUpdateName(10);
            return 1;
        }
        default:break;
    }
    // TODO: Update the input = -input - 1 case here. May change the entire function signature to take an event
    if (input == EVENT_KNOB_CHANGED) {

        

        uint8_t parameterNumber = interfaceParameters[paramPage][pot_number].number;

        static char dv[5] = {0};
        switch (interfaceParameters[paramPage][pot_number].type) {
            case parameterNone:
                return 0;
            case parameterCont:
                cpParameterList[parameterNumber] = pot_value;
                sprintf(dv, "%4d", cpParameterList[parameterNumber] >> 8);
                break;
            case parameterStep: {
                uint8_t v = pot_value >> (16 - spParameterBits[parameterNumber]);
                spParameterList[parameterNumber] = v;
                if (interfaceParameters[paramPage][pot_number].number == spOscAWave ||
                      interfaceParameters[paramPage][pot_number].number == spOscBWave) {
                        sprintf(dv, "%4d", spParameterList[parameterNumber]); 
                    } else {
                        sprintf(dv, "%4s", interfaceParameters[paramPage][pot_number].values[spParameterList[parameterNumber]]);
                    
                    }
                    
                break;

            }
            default:
                break;
        }
        if (pot_number < NUM_OF_POTS / 2) {
            lcdChangePos(pot_number * 5, 1);
        } else {
            lcdChangePos((pot_number - 4) * 5, 3);
        }
        lcdSendCharArray(dv);
    }

    return 0;
}

void interfaceParameterUpdatePage(uint8_t paramPage) {

    lcdCmd(0x01);
    delay(2);
    const char *space = " ";
    //lcdChangePos(0, 0);
    for (int i = 0; i < 4; ++i) {
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type != parameterNone) {
            lcdSendCharArray(interfaceParameters[paramPage][i].shortName);
            lcdSendCharArray(space);
        }
    }

    lcdChangePos(0, 1);
    for (int i = 0; i < 4; ++i) {
        static char dv[5] = {0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type == parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[paramPage][i].number] >> 8);
            lcdSendCharArray(dv);
            lcdSendCharArray(space);
        } else if (interfaceParameters[paramPage][i].type == parameterStep) {
            //sprintf(dv, "%4d", interfaceParameters[interface.param_page][i].values[spParameterList[interfaceParameters[interface.param_page][i].number]]);
            lcdSendCharArray(
                    interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
            lcdSendCharArray(space);
        }
    }

    lcdChangePos(0, 2);
    for (int i = 4; i < 8; ++i) {
        if (interfaceParameters[paramPage][i].type != parameterNone) {
            lcdSendCharArray(interfaceParameters[paramPage][i].shortName);
            lcdSendCharArray(space);
        }
    }

    lcdChangePos(0, 3);
    for (int i = 4; i < 8; ++i) {
        static char dv[5] = {0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type == parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[paramPage][i].number] >> 8);
            lcdSendCharArray(dv);
            lcdSendCharArray(space);
        } else if (interfaceParameters[paramPage][i].type == parameterStep) {
            //sprintf(dv, "%4d", spParameterList[interfaceParameters[interface.param_page][i].number]);
            if (!(interfaceParameters[paramPage][i].number == spOscAWave ||
                  interfaceParameters[paramPage][i].number == spOscBWave)) {
                //lcdSendCharArray(interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
                sprintf(dv, "%4s", interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
            }else{
                sprintf(dv, "%4d", spParameterList[interfaceParameters[paramPage][i].number]);
            }
            lcdSendCharArray(dv);
            lcdSendCharArray(space);
        }
    }
}

