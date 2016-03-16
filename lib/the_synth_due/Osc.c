/*
 * CFile1.c
 *
 * Created: 2/17/2016 8:40:58 PM
 *  Author: Michael Haines
 */ 
#include "Osc.h"

void setWaves(struct oscillator_struct * osc, struct Given_Voice * set_voice, byte lowest_note, byte highest_note){
	for(lowest_note; lowest_note<=highest_note;lowest_note++){
		osc->all_wavs[lowest_note] = set_voice;
	}
}

inline void osc_update(struct oscillator_struct *osc){
	osc->phase_accumulators += osc->frequancy_tuning_word + osc->Pitch_bend;
	
	if (osc->phase_accumulators >= &osc->all_wavs[osc->note]->max_length) osc->phase_accumulators -= *osc->loop_point[osc->note];

	osc->output = 127 - *(osc->all_wavs[osc->note]->wave + ((osc->phase_accumulators) >> 8));
}