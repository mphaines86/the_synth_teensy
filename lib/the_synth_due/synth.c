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


volatile uint16_t amplitude[SYNTH_VOICE_COUNT];           //-Wave amplitudes [0-255]
volatile uint16_t pitch[SYNTH_VOICE_COUNT];          //-Voice pitch
volatile uint16_t cv_pitch[SYNTH_VOICE_COUNT];

volatile unsigned char divider = 0;//-Sample rate decimator for envelope
volatile uint16_t time_hz = 0;

volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT];

volatile int noteTrigger[SYNTH_VOICE_COUNT];
volatile int noteDeath[SYNTH_VOICE_COUNT];
volatile int current_stage = 0;

volatile int32_t test_variable = 0;

const byte analog_pins[] = {ADC_CHANNEL_0, ADC_CHANNEL_1, ADC_CHANNEL_2, ADC_CHANNEL_3,
ADC_CHANNEL_4, ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7, ADC_CHANNEL_10,
ADC_CHANNEL_11,};
volatile int increament_analog_pins = 0;
volatile uint16_t last_analog_value[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

uint8_t aftertouch = 0;
uint8_t filter_cutoff = 0;
uint16_t filterEnvs_amount = 65535;
uint16_t resonantEnvs_amount = 65535;
int8_t global_detune = 0;
uint8_t cv7 = 0;
uint8_t cv70 = 0;
uint8_t cv2 = 0;
uint8_t cv12 = 0;
volatile uint16_t filter_total = 0;


 byte notes[SYNTH_VOICE_COUNT];
 int free_notes[] = {0, 1, 2, 3, 4, 5, 6, 7,};

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
  divider%=SYNTH_VOICE_COUNT;

	//-------------------------------
	// Volume and Filter envelope generator
	//-------------------------------

	envelope_update(&synthesizer.amplitudeEnvs[divider]);
	envelope_update(&synthesizer.filterEnvs[divider]);
	envelope_update(&synthesizer.resonantEnvs[divider]);

	filter_total = (env_getOutput(&synthesizer.filterEnvs[0]) + filter_cutoff + aftertouch);
	if (filter_total > 255) filter_total = 255;

	amplitude[divider] = env_getOutput(&synthesizer.amplitudeEnvs[divider]);
	REG_PIOC_ODSR ^= 512;
	REG_PIOC_ODSR = ((filter_total) << 1);
	REG_PIOC_ODSR |= 512;
	REG_PIOC_ODSR = (512 | ((env_getOutput(&synthesizer.resonantEnvs[0])) << 1));

	lfo_update(&synthesizer.pitchlfo[divider]);
	uint16_t temp_pitch[2] = {synthesizer.oscillators[divider].cv_pitch[0] + lfo_getOutput(&synthesizer.pitchlfo[divider]),
		synthesizer.oscillators[divider].cv_pitch[1] + lfo_getOutput(&synthesizer.pitchlfo[divider])};
	osc_setAllPitch(&synthesizer.oscillators[divider], temp_pitch);

	//-------------------------------
	//  Synthesizer/audio mixer
	//-------------------------------

	/*for (i=0; i < SYNTH_VOICE_COUNT; i++){
		phase_accumulators[i] += frequancy_tuning_word[i];
		//pitch[i] = CVtoFrequancy(cv_pitch[i]);
	}*/

	/*for(i=0; i < SYNTH_VOICE_COUNT; i++){
		//if (phase_accumulators[i] >= max_length[i] ){
		//	if (loop_point[i] != 0) phase_accumulators[i] -= loop_point[i];
		//	else phase_accumulators[i] = max_length[i];
		//}
		osc_update(&synthesizer.oscillators[i]);
	}
	int16_t output_sum = 0;
	int16_t wave_temp = 0;
	for (i=0; i<SYNTH_VOICE_COUNT; i++){
		//wave_temp = 127 - *(wavs[i] + ((phase_accumulators[i]) >> 9));
		//output_sum += ((wave_temp * amplitude[i]) >> 8);
		output_sum += ((osc_getOutput(&synthesizer.oscillators[i]) * amplitude[i]) >> 8);
	}*/
	int j;
	int16_t output_sum = 0;
	for(i=0; i<SYNTH_VOICE_COUNT; i++){
			osc_update(&synthesizer.oscillators[i]);
			for(j=0; j<2; j++){
					output_sum += ((osc_getOutput(&synthesizer.oscillators[i], j) * amplitude[i]) >> 8);
			}
	}

	REG_PIOD_ODSR = 127 + ((output_sum) >> 3);

	//test_variable = synthesizer.oscillators[0].pitch[0];
	//frequancy_tuning_word[divider] = pitch[divider];
	osc_updateFrequancyTuningWord(&synthesizer.oscillators[divider]);

	time_hz++;
}

void TC4_Handler(){
		TC_GetStatus(TC1, 1);
		int i;
		uint16_t value = adc_get_channel_value(ADC, analog_pins[increament_analog_pins]);
		if (last_analog_value[increament_analog_pins] - value <= 4 && last_analog_value[increament_analog_pins] - value >= -4){
			increament_analog_pins++;
			increament_analog_pins%=10;
			return;
		}

		switch(increament_analog_pins){
				case 9:
						test_variable = value;
						filter_cutoff = value >> 4;
						break;
				case 8:
						for (i = 0; i < SYNTH_VOICE_COUNT; i++){
								if (value == 127){
									envelope_setup(&synthesizer.filterEnvs[i],
										65535, synthesizer.filterEnvs[i].decayIncreament,
										synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
								}
								else{
									envelope_setup(&synthesizer.filterEnvs[i],
										value + 1, synthesizer.filterEnvs[i].decayIncreament,
										synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
								}
						}
						break;
				case 0:
						for (i = 0; i < SYNTH_VOICE_COUNT; i++){
								envelope_setup(&synthesizer.filterEnvs[i],
									synthesizer.filterEnvs[i].attackIncreament, value + 1,
									synthesizer.filterEnvs[i].sustainCV, synthesizer.filterEnvs[i].releaseIncreament);
							}
							break;

				case 1:
						for (i = 0; i < SYNTH_VOICE_COUNT; i++){
								envelope_setup(&synthesizer.filterEnvs[i],
									synthesizer.filterEnvs[i].attackIncreament, synthesizer.filterEnvs[i].decayIncreament,
									(16 * (value + 1)) - 1, synthesizer.filterEnvs[i].releaseIncreament);
							}
							break;
				case 2:
						for (i = 0; i < SYNTH_VOICE_COUNT; i++){
								envelope_setup(&synthesizer.filterEnvs[i],
									synthesizer.filterEnvs[i].attackIncreament, synthesizer.filterEnvs[i].decayIncreament,
									synthesizer.filterEnvs[i].sustainCV, value + 1);
							}
							break;

		}
		increament_analog_pins++;
		increament_analog_pins%=10;
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
		osc_setWave(&synthesizer.oscillators[i], &Arr1, 0);
		osc_setWave(&synthesizer.oscillators[i], &Arr1, 1);
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

			pmc_enable_periph_clk((uint32_t)TC4_IRQn);
			TC_Configure(TC1, 1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
			rc = VARIANT_MCK/128/1000; //128 because we selected TIMER_CLOCK4 above
			//TC_SetRA(TC1, 2, rc/2); //50% high, 50% low
			TC_SetRC(TC1, 1, rc);
			TC_Start(TC1, 1);
			TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
			TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
			NVIC_EnableIRQ(TC4_IRQn);

			pmc_enable_periph_clk(ID_ADC); // To use peripheral, we must enable clock distributon to it
			adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST); // initialize, set maximum posibble speed
			adc_disable_interrupt(ADC, 0xFFFFFFFF);
			adc_set_resolution(ADC, ADC_12_BITS);
			adc_configure_power_save(ADC, 0, 0); // Disable sleep
			adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1); // Set timings - standard values
			adc_set_bias_current(ADC, 1); // Bias current - maximum performance over current consumption
			adc_stop_sequencer(ADC); // not using it
			adc_disable_tag(ADC); // it has to do with sequencer, not using it
			adc_disable_ts(ADC); // deisable temperature sensor
			adc_configure_trigger(ADC, ADC_TRIG_SW, 1); // triggering from software, freerunning mode
			adc_disable_all_channel(ADC);
			int i = 0;
			for (i=0; i < 10; i++){
				adc_disable_channel_differential_input(ADC, analog_pins[i]);
				adc_enable_channel(ADC, analog_pins[i]); // just one channel enabled
			}
			adc_start(ADC);

			set_envelopes();
			set_lfo();
}

void note_trigger(byte channel, byte given_pitch, byte velocity){

	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
			if(free_notes[i] > -1){
					notes[i] = given_pitch;
					//notes[i+1] = given_pitch; // Second Oscillator
					switch(channel){
						case 1:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &Arr1, 0);
							//mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &Arr1, 1); // Second Oscillator
							break;
						case 2:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &Bassgt, 0);
							//mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &Bassgt, 1);
							break;
						case 3:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &Strchoir, 0);
							//mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &cello3, 1);
							break;
						case 4:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &EuroBell1, 0);
							//mTrigger(i, given_pitch, velocity * 516, 1 + global_detune, &EuroBell2, 1);
							break;
						case 5:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &bikodrum, 0);
							//mTrigger(i, given_pitch, velocity * 516, 1 + global_detune, &bikodrum, 1);
							break;
						case 6:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &saw, 0);
							//mTrigger(i, given_pitch, velocity * 516, 1 + global_detune, &saw, 1);
							break;
						default:
							mTrigger(i, given_pitch, velocity * 516, 0 + global_detune, &EuroBell1, 0);
							//mTrigger(i, given_pitch, velocity * 516, 1 + global_detune, &EuroBell1, 1); // Second Oscillator
							break;
					}
					free_notes[i] = -1;
					current_stage += 1;
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
	//REMOVE
	//envelope_setStage(&synthesizer.amplitudeEnvs[channel],RELEASE);
	current_stage -= 1;
	if (current_stage <= 0){
		envelope_setStage(&synthesizer.filterEnvs[0],RELEASE);
		envelope_setStage(&synthesizer.resonantEnvs[0],RELEASE);
		current_stage = 0;
	}
}

void set_pitch(unsigned char voice,unsigned char MIDInote, int8_t detune)
{
	cv_pitch[voice] = 240 * MIDInote + detune * 12;
	pitch[voice] = CVtoFrequancy(cv_pitch[voice]);

	//osc_setAllPitch(&synthesizer.oscillators[voice], cv_pitch[voice]);

}

void mTrigger(unsigned char voice,unsigned char MIDInote, uint16_t given_amplitude,
	int8_t detune, struct Voice * given_voice, byte oscillator)
{

	uint16_t temp_pitch[2] = {240 * (MIDInote - given_voice->pitch_from_C5),
		 240 * (MIDInote - given_voice->pitch_from_C5) + detune * 12};
	osc_trigger(&synthesizer.oscillators[voice], temp_pitch, MIDInote);
	osc_setWave(&synthesizer.oscillators[voice], given_voice, 0);
	osc_setWave(&synthesizer.oscillators[voice], given_voice, 1);
	cv_pitch[voice] = 240 * MIDInote;
	lfo_trigger(&synthesizer.pitchlfo[voice]);
	envelope_trigger(&synthesizer.amplitudeEnvs[voice], given_amplitude);
	envelope_trigger(&synthesizer.filterEnvs[0], filterEnvs_amount);
	envelope_trigger(&synthesizer.resonantEnvs[0], resonantEnvs_amount);

	noteTrigger[voice] = 1;
}

void ControlChange(byte number, byte value){
		int i = 0;
		switch(number){
			case 11:
					filter_cutoff = ((value) * 2);
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
