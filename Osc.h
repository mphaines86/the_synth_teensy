/*
 * IncFile1.h
 *
 * Created: 2/17/2016 7:59:57 PM
 *  Author: Michael Haines
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "synth.h"
#include "tables_due.h"

typedef enum
{
	osmNone, osmMaster, osmSlave
} oscSyncMode_t;

struct oscillator_struct{
	uint32_t phase_accumulators;
	uint16_t frequancy_tuning_word;
	uint16_t pitch;
	uint16_t Pitch_bend;
	uint16_t output;
	
	//uint8_t *wavs[128];
	uint32_t max_length;
	uint32_t loop_point;
	
	struct Voice *wavs[128];
};

void osc_init(struct oscillator_struct * osc, uint8_t wav,uint32_t max_length,uint32_t loop_point);
void setPitch(struct oscillator_struct * osc,uint16_t value);
void setVoice(struct oscillator_struct * osc,uint8_t value, byte lowest_note, byte highest_note);

void osc_update(struct oscillator_struct * osc);



#endif /* INCFILE1_H_ */