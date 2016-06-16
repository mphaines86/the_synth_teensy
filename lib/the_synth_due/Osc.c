/*
 * CFile1.c
 *
 * Created: 2/17/2016 8:40:58 PM
 *  Author: Michael Haines
 */
#include "Osc.h"
#include "pitch_tables.h"
#include "tables_due.h"

uint16_t CVtoFrequancy(uint16_t cv){
	uint8_t octave = cv / 2880;
	uint8_t pitch_index = (cv * 240) / 2880 - octave * 240;
	uint16_t pre_pitching = One_Octave_Pitches[pitch_index];
	return (pre_pitching>>(10 - octave));
}

void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[NUMBER_OF_OSCILLATORS], byte note){

		osc->note = note;
		byte i;
		//uint16_t pitch = 240 * note;
		for (i = 0; i < NUMBER_OF_OSCILLATORS; i++){
				osc->cv_pitch[i] = pitch[i];
				osc->pitch[i] = CVtoFrequancy(pitch[i]);
				osc->phase_accumulator[i]= 0;
		}


}

void osc_setWaves(struct oscillator_struct * osc, struct Voice * set_voice,
	byte lowest_note, byte highest_note, byte oscillator){
	for(lowest_note; lowest_note<=highest_note;lowest_note++){
		osc->all_wavs[oscillator][lowest_note] = set_voice;
	}
}

void osc_setPitch(struct oscillator_struct * osc, uint16_t value, byte oscillator){
		osc->pitch[oscillator] = CVtoFrequancy(value);
}

void osc_setAllPitch(struct oscillator_struct * osc, uint16_t value[NUMBER_OF_OSCILLATORS]){
		byte i;
		for (i = 0; i < NUMBER_OF_OSCILLATORS; i++){
				osc->pitch[i] = CVtoFrequancy(value[i]);
		}
}


void osc_setWave(struct oscillator_struct * osc, struct Voice * set_voice, byte oscillator){
		osc->all_wavs[oscillator][osc->note] = set_voice;
}

void osc_updateFrequancyTuningWord(struct oscillator_struct * osc){
	int i;
	for(i = 0; i < NUMBER_OF_OSCILLATORS; i ++){
		osc->frequancy_tuning_word[i] = osc->pitch[i];
	}
}
int16_t osc_getOutput(struct oscillator_struct * osc, byte oscillator){
	return osc->output[oscillator];
}

void osc_update(struct oscillator_struct *osc){

		int i;
		for(i = 0; i < NUMBER_OF_OSCILLATORS; i ++){
				osc->phase_accumulator[i] += osc->frequancy_tuning_word[i];

				if (osc->phase_accumulator[i] >= osc->all_wavs[i][osc->note]->max_length) {
					if (osc->all_wavs[i][osc->note]->loop_point != 0){
						osc->phase_accumulator[i] -= osc->all_wavs[i][osc->note]->loop_point;

					}
					else{
						osc->phase_accumulator[i] = osc->all_wavs[i][osc->note]->max_length;
					}
				}

				osc->output[i] = (127 - *(osc->all_wavs[i][osc->note]->wave + ((osc->phase_accumulator[i]) >> 9)));
		}
}
