/*
 * Osc.c
 *
 * Created: 2/17/2016 8:40:58 PM
 *  Author: Michael Haines
 */
#include "Osc.h"
#include "pitch_tables.h"
#include "tables_due.h"
#include "synth.h"

//Converts linear CV pitch into logarithmic pitch
uint16_t CVtoFrequancy(uint32_t cv){
	uint8_t octave = cv / 2880;
	uint8_t pitch_index = (cv * 240) / 2880 - octave * 240;
	uint32_t pre_pitching = One_Octave_Pitches[pitch_index];
	return (pre_pitching>>(10 - octave));
}

void osc_init(struct oscillator_struct * osc){


	for(uint8_t i=0; i<NUMBER_OF_OSCILLATORS; i++){
		ring_buffer_init(&osc->ringBuffer[i], osc->buffer[i], sizeof(osc->buffer[i]));
        for(uint16_t j=0; j<OSC_BUFFER_SIZE; j++){
            osc->buffer[i][j]=0;
        }
		osc->output[i] = 0;
		osc->direction[i] = 1;
		osc->oscillator_mix[i] = 0;
		osc->cv_pitch[i] = 0;
		osc->pitch[i] = 0;
		osc->phase_accumulator[i] = 0;
		osc->frequancy_tuning_word[i] = 0;
		osc->modulations[i] = 0;
		osc_setWaves(osc, &waveStruct[0], 0, 127, i);
	}
	osc->sync = osmNone;
	osc->fmModulation = 0;
	osc->amplitude=0;
}

void osc_trigger(struct oscillator_struct * osc, uint16_t pitch[NUMBER_OF_OSCILLATORS], uint8_t note,
	uint8_t osc_amp[NUMBER_OF_OSCILLATORS]){

		osc->note = note;
		byte i;
		//uint16_t pitch = 240 * note;
		for (i = 0; i < NUMBER_OF_OSCILLATORS; i++){
			osc->direction[i] = 1;
            osc->oscillator_mix[i] = osc_amp[i];
            osc->cv_pitch[i] = (pitch[i] + osc->all_wavs[i][osc->note]->pitch_from_C5) * 240;
            osc->pitch[i] = CVtoFrequancy(pitch[i]);
            osc->phase_accumulator[i]= osc->all_wavs[i][osc->note]->start_point;
		}
		//test_variable = osc->cv_pitch[0];


}

void osc_setParameters(struct oscillator_struct *osc, oscSyncMode_t sync, uint16_t oscAmix, uint16_t oscBmix){
    osc->sync = sync;
    osc->oscillator_mix[0] = oscAmix;
    osc->oscillator_mix[1] = oscBmix;
}

void osc_setWaves(struct oscillator_struct *osc, struct Voice *set_voice,
                  uint8_t lowest_note, uint8_t highest_note, uint8_t oscillator){
	uint8_t current_note;

	for(current_note = lowest_note; current_note<=highest_note;current_note++){
	    osc->all_wavs[oscillator][current_note] = set_voice;
	}
}


void osc_setWave(struct oscillator_struct *osc, struct Voice *set_voice, uint8_t oscillator){
		osc->all_wavs[oscillator][osc->note] = set_voice;
}

void osc_setSync(struct oscillator_struct * osc, oscSyncMode_t sync){
	osc->direction[1] = 1;
	osc->sync = sync;
}

void osc_updateFrequancyTuningWord(struct oscillator_struct * osc){
	int i;
	for(i = 0; i < NUMBER_OF_OSCILLATORS; i ++){
		osc->frequancy_tuning_word[i] = osc->pitch[i];
	}
}

void osc_update(struct oscillator_struct *osc){

    //for(uint8_t i = 0; i < NUMBER_OF_OSCILLATORS; i ++){

    //Oscillator 1
    //osc->frequancy_tuning_word[0] = osc->pitch[0];
    osc->frequancy_tuning_word[0] = CVtoFrequancy(osc->cv_pitch[0] + osc->modulations[0] +
    		                                      osc->output[1] * osc->fmModulation);

    osc->phase_accumulator[0] += osc->direction[0] * osc->frequancy_tuning_word[0];

    //Check to see if we reached the end of the wave
    if (osc->phase_accumulator[0] >= osc->all_wavs[0][osc->note]->end_length) {

        uint32_t test = osc->all_wavs[0][osc->note]->end_length - osc->all_wavs[0][osc->note]->loop_point;
        if (osc->sync == osmHard){
            //test_variable++;
            osc->phase_accumulator[1]=0;
        }
        osc->phase_accumulator[0] -= test;
        if (!test){
            osc->phase_accumulator[0] = osc->all_wavs[0][osc->note]->end_length;
        }


    }
    osc->output[0] = (127 - *(osc->all_wavs[0][osc->note]->wave +
        ((osc->phase_accumulator[0]) >> 9)));

    // Oscillator 2
	//osc->frequancy_tuning_word[1] = osc->pitch[1];

	osc->phase_accumulator[1] += osc->direction[1] * osc->frequancy_tuning_word[1];

	//Check to see if we reached the end of the wave
	if (osc->phase_accumulator[1] >= osc->all_wavs[1][osc->note]->end_length) {

		uint32_t test = osc->all_wavs[1][osc->note]->end_length - osc->all_wavs[1][osc->note]->loop_point;

		osc->phase_accumulator[1] -= test;
		if (!test){
			osc->phase_accumulator[1] = osc->all_wavs[1][osc->note]->end_length;
		}


	}
	osc->output[1] = (127 - *(osc->all_wavs[1][osc->note]->wave +
							  ((osc->phase_accumulator[1]) >> 9)));

	if(osc->sync == osmRing){
	    osc->output[1] = (osc->output[0] * osc->output[1]) >> 6;
	}

    ring_buffer_put(&osc->ringBuffer[0], (uint8_t) (127 + ((((osc->output[0] * osc->oscillator_mix[0]) >> 8) * osc->amplitude) >> 8)));
	ring_buffer_put(&osc->ringBuffer[1], (uint8_t) (127 + ((((osc->output[1] * osc->oscillator_mix[1]) >> 8) * osc->amplitude) >> 8)));
    //}
    //osc->output_sum = output;
}
