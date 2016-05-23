/*
 * CFile1.c
 *
 * Created: 2/12/2016 10:27:58 PM
 *  Author: Michael Haines
 */
#include <Arduino.h>
#include "tables_due.h"
#include "synth.h"
#include "Envelope.h"
#include "Osc.h"
#include "lfo.h"
//#include "pitch_tables.h"


static struct{

	struct envelope_struct amplitudeEnvs[SYNTH_VOICE_COUNT];
	struct oscillator_struct oscillators[SYNTH_VOICE_COUNT];
	struct envelope_struct filterEnvs[SYNTH_VOICE_COUNT];
	struct envelope_struct resonantEnvs[SYNTH_VOICE_COUNT];
	struct lfo_struct pitchlfo[SYNTH_VOICE_COUNT];

	} synthesizer;


volatile uint32_t phase_accumulators[SYNTH_VOICE_COUNT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};      //-Wave phase accumulators
volatile uint16_t frequancy_tuning_word[SYNTH_VOICE_COUNT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
volatile uint16_t amplitude[SYNTH_VOICE_COUNT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};           //-Wave amplitudes [0-255]
volatile uint16_t pitch[SYNTH_VOICE_COUNT] = {
  100, 500, 500, 500, 100, 500, 500, 500, 0, 0, 0, 0, 0, 0, 0, 0,};          //-Voice pitch

volatile const uint8_t *wavs[SYNTH_VOICE_COUNT];                                   //-Wave table selector [address of wave in memory]

volatile uint32_t max_length[SYNTH_VOICE_COUNT] = {
  28890112, 28890112, 28890112, 28890112, 28890112, 28890112, 28890112, 28890112,
  28890112, 28890112, 28890112, 28890112, 28890112, 28890112, 28890112, 28890112,
  };//1040384, //15043584
volatile uint32_t loop_point[SYNTH_VOICE_COUNT] = {
	2006528, 2006528, 2006528, 2006528, 2006528, 2006528, 2006528, 2006528,
	2006528, 2006528, 2006528, 2006528, 2006528, 2006528, 2006528, 2006528,
};

volatile unsigned char divider = 0;//-Sample rate decimator for envelope
volatile uint16_t time_hz = 0;
volatile unsigned char tik = 0;
volatile unsigned char output_mode;

volatile uint16_t wave_amplitude[SYNTH_VOICE_COUNT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

 volatile int noteTrigger[SYNTH_VOICE_COUNT] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

volatile int noteDeath[SYNTH_VOICE_COUNT] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
volatile uint16_t cv_pitch[SYNTH_VOICE_COUNT] = {
  100, 500, 500, 500, 100, 500, 500, 500, 0, 0, 0, 0, 0, 0, 0, 0,};

volatile int current_stage = 0;
volatile byte env_set = 0;
volatile int32_t test_variable = 0;

volatile uint8_t aftertouch = 0;
volatile uint8_t filter_cutoff = 0;
volatile uint16_t filterEnvs_amount = 65535;
volatile uint16_t resonantEnvs_amount = 65535;
volatile int8_t global_detune = 0;
volatile uint8_t cv7 = 0;
volatile uint8_t cv70 = 0;
volatile uint8_t cv2 = 0;
volatile uint8_t cv12 = 0;
volatile uint16_t filter_total = 0;


volatile byte notes[SYNTH_VOICE_COUNT];
volatile int free_notes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,};

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************

void TC5_Handler(){
	TC_GetStatus(TC1, 2);

	int i = 0;
	//-------------------------------
	// Time division
	//-------------------------------
	divider++;
	if(!(divider&=0x0f))
		tik=1;

	//-------------------------------
	// Volume and Filter envelope generator
	//-------------------------------
	/*if(noteTrigger[divider]){

		envelope_trigger(&synthesizer.amplitudeEnvs[divider], wave_amplitude[divider]);
		envelope_trigger(&synthesizer.filterEnvs[1], 65535);
		envelope_trigger(&synthesizer.resonantEnvs[1], 65535);
		noteTrigger[divider] = 0;
		current_stage++;
	}*/

	/*else if (noteDeath[divider] == 1){
		envelope_setStage(&synthesizer.amplitudeEnvs[divider],RELEASE);
		noteDeath[divider] = 0;
		current_stage--;
		if(current_stage <= 0){
			envelope_setStage(&synthesizer.filterEnvs[1],RELEASE);
			envelope_setStage(&synthesizer.resonantEnvs[1],RELEASE);
			current_stage = 0;
		}
	}*/
	envelope_update(&synthesizer.amplitudeEnvs[divider]);
	envelope_update(&synthesizer.filterEnvs[divider]);
	envelope_update(&synthesizer.resonantEnvs[divider]);

	filter_total = (env_getOutput(&synthesizer.filterEnvs[1]) + filter_cutoff + aftertouch);
	if (filter_total > 255) filter_total = 255;

	amplitude[divider] = env_getOutput(&synthesizer.amplitudeEnvs[divider]);
	REG_PIOC_ODSR ^= 512;
	REG_PIOC_ODSR = ((filter_total) << 1);
	REG_PIOC_ODSR |= 512;
	REG_PIOC_ODSR = (512 | ((env_getOutput(&synthesizer.resonantEnvs[1])) << 1));
	//REG_PIOC_ODSR ^= ((4864)|((env_getOutput(&synthesizer.resonantEnvs[1])) << 1));
	//REG_PIOC_ODSR = (4608) ^ ((4096)|((env_getOutput(&synthesizer.resonantEnvs[1])) << 1));
	//test_variable = (4608) ^ ((4096)|((env_getOutput(&synthesizer.resonantEnvs[1])) << 1));
	lfo_update(&synthesizer.pitchlfo[divider]);
	test_variable = lfo_getOutput(&synthesizer.pitchlfo[1]);
	pitch[divider] = CVtoFrequancy(cv_pitch[divider] + lfo_getOutput(&synthesizer.pitchlfo[divider]));

	//-------------------------------
	//  Synthesizer/audio mixer
	//-------------------------------

	for (i=0; i < SYNTH_VOICE_COUNT; i++){
		phase_accumulators[i] += frequancy_tuning_word[i];
		//pitch[i] = CVtoFrequancy(cv_pitch[i]);
	}

	for(i=0; i < SYNTH_VOICE_COUNT; i++){
		if (phase_accumulators[i] >= max_length[i] ){
			if (loop_point[i] != 0) phase_accumulators[i] -= loop_point[i];
			else phase_accumulators[i] = max_length[i];
		}
	}
	int16_t output_sum = 0;
	int16_t wave_temp = 0;
	for (i=0; i<SYNTH_VOICE_COUNT; i++){
		wave_temp = 127 - *(wavs[i] + ((phase_accumulators[i]) >> 9));
		output_sum += ((wave_temp * amplitude[i]) >> 8);
	}

	/*for (i=1; i<SYNTH_VOICE_COUNT; i+= 2){
		wave_temp = 127 - *(wavs[i] + ((phase_accumulators[i]) >> 9));
		output_sum += ((wave_temp * amplitude[i/2]) >> 8);
	}*/


	REG_PIOD_ODSR = 127 + ((output_sum) >> 3);
	//test_variable = 127 + ((output_sum) >> 2);
	frequancy_tuning_word[divider] = pitch[divider];
	time_hz++;
}

void set_envelopes(){

	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
		envelope_setup(&synthesizer.amplitudeEnvs[i], 65535,65535,65535,15);
		envelope_setup(&synthesizer.filterEnvs[i], 22,78,43234,1);
		//envelope_setup(&synthesizer.resonantEnvs[i], 46,56,45333,19);
		envelope_setup(&synthesizer.resonantEnvs[i], 65535,75,45345,19);
	}

}


void set_oscillators(){
	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT - 1; i++){
		//setVoices(&synthesizer.oscillators[i], &string_C6, 0, 127);
	}

	//setVoices(&synthesizer.oscillators[7], &snare,0,127);

}

void set_lfo(){
	int i = 0;
	for (i = 0; i< SYNTH_VOICE_COUNT; i++){
		lfo_init(&synthesizer.pitchlfo[i], lfoSine, 47, 89);
	}
}

void begin()
{
			pmc_set_writeprotect(false);
			pmc_enable_periph_clk((uint32_t)TC5_IRQn);
			TC_Configure(TC1, 2, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
			uint32_t rc = VARIANT_MCK/128/FS; //128 because we selected TIMER_CLOCK4 above
			//TC_SetRA(TC1, 2, rc/2); //50% high, 50% low
			TC_SetRC(TC1, 2, rc);
			TC_Start(TC1, 2);
			TC1->TC_CHANNEL[2].TC_IER=TC_IER_CPCS;
			TC1->TC_CHANNEL[2].TC_IDR=~TC_IER_CPCS;
			NVIC_EnableIRQ(TC5_IRQn);


			set_envelopes();
			set_lfo();
}

void note_trigger(byte channel, byte given_pitch, byte velocity){
	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i+=2){
			if(free_notes[i] > -1){
					notes[i] = given_pitch;
					notes[i+1] = given_pitch; // Second Oscillator
					switch(channel){
						case 1:
							mTrigger(i, given_pitch, velocity * 516, 0, &Arr1);
							mTrigger(i + 1, given_pitch, velocity * 516, 0 + global_detune, &Swannee); // Second Oscillator
							break;
						case 2:
							mTrigger(i, given_pitch, velocity * 516, 0, &Bassgt);
							mTrigger(i + 1, given_pitch, velocity * 516, 0 + global_detune, &Bassgt);
							break;
						case 3:
							mTrigger(i, given_pitch, velocity * 516, 0, &Strchoir);
							mTrigger(i + 1, given_pitch, velocity * 516, 0 + global_detune, &cello3);
							break;
						case 4:
							mTrigger(i, given_pitch, velocity * 516, 0, &EuroBell1);
							mTrigger(i + 1, given_pitch, velocity * 516, 1 + global_detune, &EuroBell2);
							break;
						case 5:
							mTrigger(i, given_pitch, velocity * 516, 0, &bikodrum);
							mTrigger(i + 1, given_pitch, velocity * 516, 1 + global_detune, &bikodrum);
							break;
						case 6:
							mTrigger(i, given_pitch, velocity * 516, 0, &saw);
							mTrigger(i + 1, given_pitch, velocity * 516, 1 + global_detune, &saw);
							break;
						default:
							mTrigger(i, given_pitch, velocity * 516, 0, &EuroBell1);
							mTrigger(i + 1, given_pitch, velocity * 516, 1 + global_detune, &EuroBell1); // Second Oscillator
							break;
					}
					free_notes[i] = -1;
					free_notes[i + 1] = -1; // Second Oscillator
					current_stage += 2;
					break;
			}
	}
}


void NoteRelease(byte channel, byte given_pitch, byte velocity){
	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
			if(notes[i] == given_pitch){
				  noteDeath[i] = 1;
					free_notes[i] = i;

					envelope_setStage(&synthesizer.amplitudeEnvs[i],RELEASE);
		}

	}
	current_stage -= 2;
	if (current_stage <= 0){
		envelope_setStage(&synthesizer.filterEnvs[1],RELEASE);
		envelope_setStage(&synthesizer.resonantEnvs[1],RELEASE);
		current_stage = 0;
	}
}

void set_pitch(unsigned char voice,unsigned char MIDInote, int8_t detune)
{
	cv_pitch[voice] = 240 * MIDInote + detune * 12;
	pitch[voice] = CVtoFrequancy(cv_pitch[voice]);

}

void mTrigger(unsigned char voice,unsigned char MIDInote, uint16_t given_amplitude, int8_t detune, struct Voice * given_voice)
{
	/*if (MIDInote <= 36)
	{
		wavs[voice] = piano_C3.wave;
		max_length[voice] = piano_C3.max_length;
		loop_point[voice] = piano_C3.loop_point;
		//pitch[voice]=*(&PITCHS[MIDInote - piano_C3.pitch_from_C5]);
		setPitch(voice,MIDInote - piano_C3.pitch_from_C5, 0 );
	}
	if (MIDInote > 36 && MIDInote <= 55)
	{
		wavs[voice] = piano_C5.wave;
		max_length[voice] = piano_C5.max_length;
		loop_point[voice] = piano_C5.loop_point;
		//pitch[voice]=*(&PITCHS[MIDInote - piano_C5.pitch_from_C5]);
		setPitch(voice,MIDInote - piano_C5.pitch_from_C5, 0 );
	}
	if (MIDInote > 55)
	{
		wavs[voice] = piano_C6.wave;
		max_length[voice] = piano_C6.max_length;
		loop_point[voice] = piano_C6.loop_point;
		//pitch[voice]=*(&PITCHS[MIDInote - piano_C6.pitch_from_C5]);
		setPitch(voice,MIDInote - piano_C6.pitch_from_C5, 0 );
	}*/

	wavs[voice] = given_voice->wave;
	max_length[voice] = given_voice->max_length;
	loop_point[voice] = given_voice->loop_point;
	set_pitch(voice, MIDInote - given_voice->pitch_from_C5, detune);
	lfo_trigger(&synthesizer.pitchlfo[voice]);
	envelope_trigger(&synthesizer.amplitudeEnvs[voice], wave_amplitude[voice]);
	envelope_trigger(&synthesizer.filterEnvs[1], filterEnvs_amount);
	envelope_trigger(&synthesizer.resonantEnvs[1], resonantEnvs_amount);

	phase_accumulators[voice] = 0;
	wave_amplitude[voice] = given_amplitude;
	//frequancy_tuning_word[divider] = pitch[voice];
	noteTrigger[voice] = 1;
}

void ControlChange(byte number, byte value){
		int i = 0;
		switch(number){
			case 11:
					filter_cutoff = ((value) * 2) + 1;
					break;
			case 2:
					cv2 = value;
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							lfo_setRate(&synthesizer.pitchlfo[i], cv2 * cv70 * 2);
						}
						break;
			case 70:
					cv70 = value;
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							lfo_setRate(&synthesizer.pitchlfo[i], cv2 * cv70 * 2);
						}
						break;
			case 12:
					cv12 = value;
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							lfo_init(&synthesizer.pitchlfo[i], synthesizer.pitchlfo[i].shape, cv12 * cv7 * 2, synthesizer.pitchlfo[i].rate);
						}
						break;
			case 7:
					cv7 = value;
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							lfo_init(&synthesizer.pitchlfo[i], synthesizer.pitchlfo[i].shape, cv12 * cv7 * 2, synthesizer.pitchlfo[i].rate);
						}
						break;
			case 13:
					global_detune = ((value + 1) * .5) - 32;
					break;
			case 5:
					filterEnvs_amount = (512 * (value + 1)) - 1;
					break;
			case 10:
					resonantEnvs_amount = (512 * (value + 1)) - 1;
					break;
			case 71:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							if (value == 127){
								envelope_setup(&synthesizer.resonantEnvs[i],
									65535, synthesizer.resonantEnvs[i].decayIncreament,
									synthesizer.resonantEnvs[i].sustainCV, synthesizer.resonantEnvs[i].releaseIncreament);
								}
							else{
							envelope_setup(&synthesizer.resonantEnvs[i],
								(15 * (value)) + 1, synthesizer.resonantEnvs[i].decayIncreament,
								synthesizer.resonantEnvs[i].sustainCV, synthesizer.resonantEnvs[i].releaseIncreament);
							}
						}
						break;
			case 72:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.resonantEnvs[i],
								synthesizer.resonantEnvs[i].attackIncreament, (15 * (value )) + 1,
								synthesizer.resonantEnvs[i].sustainCV, synthesizer.resonantEnvs[i].releaseIncreament);
						}
						break;
			case 73:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.resonantEnvs[i],
								synthesizer.resonantEnvs[i].attackIncreament, synthesizer.resonantEnvs[i].decayIncreament,
								(512 * (value + 1)) - 1, synthesizer.resonantEnvs[i].releaseIncreament);
						}
						break;
			case 74:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.resonantEnvs[i],
								synthesizer.resonantEnvs[i].attackIncreament, synthesizer.resonantEnvs[i].decayIncreament,
								synthesizer.resonantEnvs[i].sustainCV, (12 * (value )) + 1);
						}
						break;
			case 75:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							if (value == 127){
								envelope_setup(&synthesizer.filterEnvs[i],
									65535, synthesizer.filterEnvs[i].decayIncreament,
									synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
							}
							else{
								envelope_setup(&synthesizer.filterEnvs[i],
									(15 * (value)) + 1, synthesizer.filterEnvs[i].decayIncreament,
									synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
							}
						}
						break;
			case 76:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.filterEnvs[i],
								synthesizer.filterEnvs[i].attackIncreament, (15 * (value)) + 1,
								synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
						}
						break;
			case 77:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.filterEnvs[i],
								synthesizer.filterEnvs[i].attackIncreament, synthesizer.filterEnvs[i].decayIncreament,
								(512 * (value + 1)) - 1, synthesizer.filterEnvs[i].releaseIncreament);
						}
						break;
			case 78:
					for (i = 0; i < SYNTH_VOICE_COUNT; i++){
							envelope_setup(&synthesizer.filterEnvs[i],
								synthesizer.filterEnvs[i].attackIncreament, synthesizer.filterEnvs[i].decayIncreament,
								synthesizer.filterEnvs[i].sustainCV, (12 * (value )) + 1);
						}
						break;
		}
}
