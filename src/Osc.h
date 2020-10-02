/*
 * Osc.h
 *
 * Created: 2/17/2016 7:59:57 PM
 *  Author: Michael Haines
 */


#ifndef OSC_H
#define OSC_H

#define NUMBER_OF_OSCILLATORS 2
#define OSC_BUFFER_SIZE 1024

#include "Arduino.h"
#include "system/ring_buffer.h"

//part of planned feature to sync phase of voices
typedef enum
{
	osmNone, osmHard, osmRing, osmSoft
} oscSyncMode_t;

struct oscillator_struct{
	oscSyncMode_t sync;
	uint32_t phase_accumulator[NUMBER_OF_OSCILLATORS];
	uint32_t frequancy_tuning_word[NUMBER_OF_OSCILLATORS];
	uint32_t pitch[NUMBER_OF_OSCILLATORS];
	uint32_t cv_pitch[NUMBER_OF_OSCILLATORS];
	int32_t modulations[NUMBER_OF_OSCILLATORS];
	uint16_t fmModulation;
	int16_t output[NUMBER_OF_OSCILLATORS];
	uint16_t oscillator_mix[NUMBER_OF_OSCILLATORS];
	uint16_t amplitude;
	uint8_t note;
	int8_t direction[NUMBER_OF_OSCILLATORS];
	uint16_t parameter[NUMBER_OF_OSCILLATORS];


	// Pointer from each possible note assignment to wave sample
	struct Voice *all_wavs[NUMBER_OF_OSCILLATORS][128];

	uint16_t buffer[NUMBER_OF_OSCILLATORS][OSC_BUFFER_SIZE];
	struct ring_buffer_t ringBuffer[NUMBER_OF_OSCILLATORS];

	struct {
	    uint32_t start[NUMBER_OF_OSCILLATORS];
	    uint32_t end[NUMBER_OF_OSCILLATORS];
	    uint32_t table_start[NUMBER_OF_OSCILLATORS];
	    uint32_t table_end[NUMBER_OF_OSCILLATORS];
	    uint32_t table_accumulator[NUMBER_OF_OSCILLATORS];
	    uint32_t table_adder[NUMBER_OF_OSCILLATORS];
	    uint16_t table_location[NUMBER_OF_OSCILLATORS];
	} wavetable;

};

uint16_t CVtoFrequancy(uint32_t cv);
void osc_init(struct oscillator_struct * osc);
void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[NUMBER_OF_OSCILLATORS], uint8_t note,
	uint8_t osc_amp[NUMBER_OF_OSCILLATORS]);
void osc_setModulation(struct oscillator_struct *osc, uint32_t value, uint8_t oscillator);
void osc_updateFrequancyTuningWord(struct oscillator_struct * osc);
void osc_setWaves(struct oscillator_struct *osc, struct Voice *set_voice,
				  uint8_t lowest_note, uint8_t highest_note, uint8_t oscillator);
void osc_setWave(struct oscillator_struct *osc, struct Voice *set_voice, uint8_t oscillator);
void osc_setSync(struct oscillator_struct * osc, oscSyncMode_t sync);
void osc_update(struct oscillator_struct * osc);

void osc_setParameters(struct oscillator_struct *osc, oscSyncMode_t sync, uint16_t oscAmix, uint16_t oscBmix);

inline void osc_setModulation(struct oscillator_struct *osc, uint32_t value, byte oscillator){
	osc->modulations[oscillator] = value;
	osc->frequancy_tuning_word[oscillator] = CVtoFrequancy(osc->cv_pitch[oscillator] + osc->modulations[oscillator]);
}

inline void osc_setWaveParameters(struct oscillator_struct *osc, uint16_t ramp_mod, int32_t lfo_mod, byte oscillator) {
	//value = value >> 5;
	//osc->wavetable.table_adder[oscillator] = ramp_mod >> 4;
	//osc->wavetable.table_location[oscillator] =
	int32_t value  = (ramp_mod + lfo_mod) * 262144 + osc->wavetable.table_start[oscillator];
	if(value > 16777216){
		osc->wavetable.start[oscillator] = 16515072;
		osc->wavetable.end[oscillator] = 16777216;
	}
	else if (value < 0){
		osc->wavetable.start[oscillator] = 0;
		osc->wavetable.end[oscillator] = 262144;
	}
	else {
		osc->wavetable.start[oscillator] = value;
		osc->wavetable.end[oscillator] = value + 262144;
	}
}

inline void osc_setAmplitude(struct oscillator_struct * osc, uint16_t amplitude){
	osc->amplitude = amplitude;
}

inline void osc_setTableValues(struct oscillator_struct * osc, uint16_t start, uint16_t end, uint8_t oscillator){
	if(start > end){
	    end = start;
	}

    osc->wavetable.table_start[oscillator] = (start >> 10) * 262144;
	osc->wavetable.table_end[oscillator] = ((end >> 10) + 1) * 262144;
}

//inline int16_t osc_getOutput(struct oscillator_struct * osc){
//	return osc->output_sum;
//}

inline int16_t osc_getOutput(struct oscillator_struct * osc, uint8_t number){
	return osc->output[number];
}

inline void osc_setFMModulation(struct oscillator_struct * osc, uint16_t mod){
    osc->fmModulation = mod;
}

#endif
