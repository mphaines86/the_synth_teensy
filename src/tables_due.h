/*Generated using python file "Generate_Tables.py"*/
#ifndef TABLES_DUE_H
#define TABLES_DUE_H
#include <Arduino.h>

#define MAX_WAVE_COUNT 16
struct Voice {
    const uint32_t max_length;
	const uint32_t loop_point;
	const int8_t pitch_from_C5;
    
	const uint8_t *wave;
    };
    
    extern struct Voice waveStruct[MAX_WAVE_COUNT];

#endif