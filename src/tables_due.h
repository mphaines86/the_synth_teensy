/*Generated using python file "Generate_Tables.py"*/
#ifndef TABLES_DUE_H
#define TABLES_DUE_H
#include <Arduino.h>

#define MAX_WAVE_COUNT 16
struct Voice {
	const char * name; // 8 Characters + zero
	uint32_t start_point;
    uint32_t end_length;
	uint32_t loop_point;
	int8_t pitch_from_C5;
    
	const uint8_t *wave;
	const uint32_t max_length;
    };
    
extern struct Voice waveStruct[MAX_WAVE_COUNT];
extern struct Voice waveTableStruct[MAX_WAVE_COUNT];

#endif