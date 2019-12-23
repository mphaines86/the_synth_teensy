/*
 * Osc.h
 *
 * Created: 2/17/2016 7:59:57 PM
 *  Author: Michael Haines
 */


#ifndef OSC_H
#define OSC_H
#define NUMBER_OF_OSCILLATORS 2

#include "Arduino.h"

//part of planned feature to sync phase of voices
typedef enum
{
	osmNone, osmHard, osmSoft
} oscSyncMode_t;

struct oscillator_struct{
	oscSyncMode_t sync;
	uint32_t phase_accumulator[NUMBER_OF_OSCILLATORS];
	uint32_t frequancy_tuning_word[NUMBER_OF_OSCILLATORS];
	uint32_t pitch[NUMBER_OF_OSCILLATORS];
	uint32_t cv_pitch[NUMBER_OF_OSCILLATORS];
	int16_t output_sum;
	int16_t output[NUMBER_OF_OSCILLATORS];
	uint16_t oscillator_mix[NUMBER_OF_OSCILLATORS];
	uint16_t amplitude;
	uint8_t note;
	int8_t direction[NUMBER_OF_OSCILLATORS];
	struct Voice *current_wave[NUMBER_OF_OSCILLATORS];

	// Pointer from each possible note assignment to wave sample
	struct Voice *all_wavs[NUMBER_OF_OSCILLATORS][128];

};

uint16_t CVtoFrequancy(uint32_t cv);
void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[NUMBER_OF_OSCILLATORS], uint8_t note,
	uint8_t osc_amp[NUMBER_OF_OSCILLATORS]);
void osc_setPitch(struct oscillator_struct * osc, uint32_t value, uint8_t oscillator);
void osc_updateFrequancyTuningWord(struct oscillator_struct * osc);
void osc_setWaves(struct oscillator_struct *osc, struct Voice *set_voice,
				  uint8_t lowest_note, uint8_t highest_note, uint8_t oscillator);
void osc_setWave(struct oscillator_struct *osc, struct Voice *set_voice, uint8_t oscillator);
void osc_setSync(struct oscillator_struct * osc, oscSyncMode_t sync);
void osc_update(struct oscillator_struct * osc);

void osc_setParameters(struct oscillator_struct *osc, oscSyncMode_t sync, uint16_t oscAmix, uint16_t oscBmix);

inline void osc_setPitch(struct oscillator_struct * osc, uint32_t value, byte oscillator){
	osc->pitch[oscillator] = CVtoFrequancy(value);
}

inline void osc_setAmplitude(struct oscillator_struct * osc, uint16_t amplitude){
	osc->amplitude = amplitude;
}

//inline int16_t osc_getOutput(struct oscillator_struct * osc){
//	return osc->output_sum;
//}

inline int16_t osc_getOutput(struct oscillator_struct * osc, uint8_t number){
	return osc->output[number];
}

#endif
