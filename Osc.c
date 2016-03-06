/*
 * CFile1.c
 *
 * Created: 2/17/2016 8:40:58 PM
 *  Author: Michael Haines
 */ 
#include "Osc.h"

inline void osc_update(struct oscillator_struct *osc){
	osc->phase_accumulators += osc->frequancy_tuning_word + osc->Pitch_bend;
	
	if (osc->phase_accumulators >= osc->max_length) osc->phase_accumulators -= osc->loop_point;

	//uint16_t osc->output = 127 - *(wavs[0] + ((phase_accumulators[0]) >> 8));
}