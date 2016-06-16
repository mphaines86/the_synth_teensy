/*
 * IncFile1.h
 *
 * Created: 2/17/2016 7:59:57 PM
 *  Author: Michael Haines
 */


#ifndef OSC_H
#define OSC_H
#define NUMBER_OF_OSCILLATORS 2

#include "synth.h"

typedef enum
{
	osmNone, osmMaster, osmSlave
} oscSyncMode_t;


struct Given_Voice {
	uint32_t max_length;
	uint32_t loop_point;
	uint8_t pitch_from_C5;

	uint8_t *wave;
};

struct oscillator_struct{
	uint32_t phase_accumulator[NUMBER_OF_OSCILLATORS];
	uint16_t frequancy_tuning_word[NUMBER_OF_OSCILLATORS];
	uint16_t pitch[NUMBER_OF_OSCILLATORS];
	uint16_t cv_pitch[NUMBER_OF_OSCILLATORS];
	int16_t output[NUMBER_OF_OSCILLATORS];
	byte note;

	struct Voice *all_wavs[NUMBER_OF_OSCILLATORS][128];

};

uint16_t CVtoFrequancy(uint16_t cv);
void osc_init(struct oscillator_struct * osc, uint8_t wav,uint32_t max_length,uint32_t loop_point);
void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[], byte note);
void osc_setPitch(struct oscillator_struct * osc, uint16_t value, byte oscillator);
void osc_setAllPitch(struct oscillator_struct * osc, uint16_t value[]);
void osc_updateFrequancyTuningWord(struct oscillator_struct * osc);
int16_t osc_getOutput(struct oscillator_struct * osc, byte oscillator);
void osc_setWaves(struct oscillator_struct * osc, struct Voice * set_voice, byte lowest_note, byte highest_note, byte oscillator);
void osc_setWave(struct oscillator_struct * osc, struct Voice * set_voice, byte oscillator);
void osc_update(struct oscillator_struct * osc);



#endif /* INCFILE1_H_ */
