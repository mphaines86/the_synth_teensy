/*
 * IncFile1.h
 *
 * Created: 2/17/2016 7:59:57 PM
 *  Author: Michael Haines
 */ 


#ifndef OSC_H
#define OSC_H

#include "synth.h"
//#include "tables_due.h"

typedef enum
{
	osmNone, osmMaster, osmSlave
} oscSyncMode_t;

struct Voice {
	uint32_t max_length;
	uint32_t loop_point;
	uint8_t pitch_from_C5;

	uint8_t *wave;
};

struct oscillator_struct{
	uint32_t phase_accumulators;
	uint16_t frequancy_tuning_word;
	uint16_t pitch;
	uint16_t Pitch_bend;
	uint16_t output;
	byte note;
	
	uint32_t *max_length[128];
	uint32_t *loop_point[128];
	struct Voice *all_wavs[128];
};

void osc_init(struct oscillator_struct * osc, uint8_t wav,uint32_t max_length,uint32_t loop_point);
void setPitch(struct oscillator_struct * osc,uint16_t value);
//void setWavs(struct oscillator_struct * osc,uint8_t value, byte lowest_note, byte highest_note);
void setVoices(struct oscillator_struct * osc, struct Voice * set_voice, byte lowest_note, byte highest_note);
void setVoice(struct oscillator_struct * osc,uint8_t value, byte lowest_note, byte highest_note);
void osc_update(struct oscillator_struct * osc);



#endif /* INCFILE1_H_ */