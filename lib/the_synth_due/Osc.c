/*
 * Osc.c
 *
 * Created: 2/17/2016 8:40:58 PM
 *  Author: Michael Haines
 */
#include "Osc.h"
#include "pitch_tables.h"
#include "tables_due.h"

//Converts linear CV pitch into logarithmic pitch
uint16_t CVtoFrequancy(uint16_t cv){
	uint8_t octave = cv / 2880;
	uint8_t pitch_index = (cv * 240) / 2880 - octave * 240;
	uint16_t pre_pitching = One_Octave_Pitches[pitch_index];
	return (pre_pitching>>(10 - octave));
}

void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[NUMBER_OF_OSCILLATORS], byte note,
	uint8_t osc_amp[NUMBER_OF_OSCILLATORS]){

		osc->note = note;
		byte i;
		//uint16_t pitch = 240 * note;
		for (i = 0; i < NUMBER_OF_OSCILLATORS; i++){
				osc->oscillator_mix[i] = osc_amp[i];
				osc->cv_pitch[i] = (pitch[i] + osc->all_wavs[i][osc->note]->pitch_from_C5) * 240;
				osc->pitch[i] = CVtoFrequancy(pitch[i]);
				osc->phase_accumulator[i]= 0;
		}


}

void osc_setWaves(struct oscillator_struct * osc, struct Voice * set_voice,
	byte lowest_note, byte highest_note, byte oscillator){
	uint8_t current_note;
	for(current_note = lowest_note; current_note<=highest_note;current_note++){
		osc->all_wavs[oscillator][current_note] = set_voice;
	}
}

void osc_setPitch(struct oscillator_struct * osc, uint16_t value, byte oscillator){
		osc->pitch[oscillator] = CVtoFrequancy(value);
}

void osc_setWave(struct oscillator_struct * osc, struct Voice * set_voice, byte oscillator){
		osc->all_wavs[oscillator][osc->note] = set_voice;
}

void osc_setSync(struct oscillator_struct * osc, oscSyncMode_t sync){
	osc->sync = sync;
}

void osc_setAmplitude(struct oscillator_struct * osc, uint16_t amplitude){
	osc->amplitude = amplitude;
}

void osc_updateFrequancyTuningWord(struct oscillator_struct * osc){
	int i;
	for(i = 0; i < NUMBER_OF_OSCILLATORS; i ++){
		osc->frequancy_tuning_word[i] = osc->pitch[i];
	}
}
int16_t osc_getOutput(struct oscillator_struct * osc){
	return osc->output;
}

void osc_update(struct oscillator_struct *osc){

		int16_t output = 0;
		uint8_t i;
		for(i = 0; i < NUMBER_OF_OSCILLATORS; i ++){
				osc->phase_accumulator[i] += osc->frequancy_tuning_word[i];

				//Check to see if we reached the end of the wave
				if (osc->phase_accumulator[i] >= osc->all_wavs[i][osc->note]->max_length) {
					if (osc->all_wavs[i][osc->note]->loop_point){
						osc->phase_accumulator[i] -= osc->all_wavs[i][osc->note]->loop_point;
						if (i==0 && osc->sync){
							test_variable++;
							osc->phase_accumulator[1]=0;
						}
					}
					else{
						osc->phase_accumulator[i] = osc->all_wavs[i][osc->note]->max_length;

					}
				}

				output += (((((127 - *(osc->all_wavs[i][osc->note]->wave +
					((osc->phase_accumulator[i]) >> 9))) * osc->oscillator_mix[i]) >> 8) *
					osc->amplitude) >> 8);

		}
		osc->output = output;
}
