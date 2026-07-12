#include "parameter.h"

uint16_t cpParameterList[cpCount];
uint16_t spParameterList[spCount];

uint8_t spParameterBits[spCount] = {
    4, // wave A
    4, // wave B
    2, // sync
    1, // A Type
    1, // B Type
    1, // fltr
    1, //
    1, // Amp
    1, //
    1, // Aux
    1, //
    2, // LFO A wave
    1, // LFO A
    1, // LFO A
    1, // LFO A
    2, // LFO B
    1, // LFO B
    1, // LFO B
    1, // LFO B

};