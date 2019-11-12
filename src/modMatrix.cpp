//
// Created by michael on 5/19/19.
//
#include "modMatrix.h"
#include "synth.h"
#include "Interface/interface.h"

struct modMatrixSource_s {
    uint8_t number;
    uint32_t * source;
    uint8_t resolution;
};
