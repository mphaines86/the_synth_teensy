/*
 * CFile1.c
 *
 * Created: 2/12/2016 10:27:58 PM
 *  Author: Michael Haines
 */ 
#include <Arduino.h>
#include "synth.h"
#include "Envelope.h"

static struct{
	
	struct envelope_struct amplitudeEnvs[SYNTH_VOICE_COUNT];
	
	} synthesizer;


volatile uint32_t phase_accumulators[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};      //-Wave phase accumulators
volatile uint16_t frequancy_tuning_word[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
volatile uint16_t amplitude[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};           //-Wave amplitudes [0-255]
volatile uint16_t pitch[8] = {
  100, 500, 500, 500, 100, 500, 500, 500};          //-Voice pitch
volatile int modulation[8] = {
  20, 0, 64, 127, 20, 0, 64, 127};                         //-Voice envelope modulation [0-1023 512=no mod. <512 pitch down >512 pitch up]
volatile uint8_t *wavs[8];                                   //-Wave table selector [address of wave in memory]
volatile uint16_t *envs[8];                                  //-Envelope selector [address of envelope in memory]
volatile uint16_t  envelope_phase[8] = {
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000}; //-Envelope phase accumulator
volatile uint16_t  env_fast_tuning_word[8] = {
  10, 10, 10, 10, 10, 10, 10, 10};               //-Envelope speed tuning word

volatile uint32_t max_length[8] = {
  8328448, 8328448, 8328448, 8328448, 8328448, 8328448, 8328448, 1040384,
};
volatile uint32_t loop_point[8] = {
	8328448, 8328448, 8328448, 8328448, 8328448, 8328448, 8328448, 0,
};

volatile unsigned char divider = 0;//-Sample rate decimator for envelope
volatile uint16_t time_hz = 0;
volatile unsigned char tik = 0; 
volatile unsigned char output_mode;

volatile uint8_t wave_amplitude[8] = {
  0, 0, 0, 0, 0, 0, 0, 0,
};
volatile uint16_t Pitch_bend[8] = {
  0, 0, 0, 0, 0, 0, 0, 0,};
  
 volatile int noteTrigger[8] = {
 0, 0, 0, 0, 0, 0, 0, 0,};
volatile int current_stage = 0;

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************

void TC5_Handler()
	{

	TC_GetStatus(TC1, 2);
	//-------------------------------
	// Time division
	//-------------------------------
	divider++;
	if(!(divider&=0x07))
		tik=1;

	//-------------------------------
	// Volume envelope generator
	//-------------------------------
	if(noteTrigger[divider] && env_getStage(&synthesizer.amplitudeEnvs[divider])==DEAD){
		envelope_trigger(&synthesizer.amplitudeEnvs[divider]);
	}
	else{
		envelope_update(&synthesizer.amplitudeEnvs[divider]);
		amplitude[divider] = env_getOutput(&synthesizer.amplitudeEnvs[divider])&(wave_amplitude[divider]);
		current_stage = env_getStage(&synthesizer.amplitudeEnvs[0]);
		//Serial.println(amplitude[divider]);
	}
  
	//if (!(envelope_phase[divider]&0x80)){
		//envelope_phase[divider] +=env_fast_tuning_word[divider];
		//amplitude[divider] = (*(envs[divider] + (envelope_phase[divider]>>8)))&(wave_amplitude[divider]);
    
	//AMP[divider] = pgm_read_byte(envs[divider] + (((unsigned char*)&(EPCW[divider]+=EFTW[divider]))[1]));
	//}
	//else
		//amplitude[divider] = 0;

	//-------------------------------
	//  Synthesizer/audio mixer
	//-------------------------------
	if (phase_accumulators[7] >= max_length[7]) frequancy_tuning_word[7] = 0;
 
	phase_accumulators[0] += frequancy_tuning_word[0] + Pitch_bend[0];
	phase_accumulators[1] += frequancy_tuning_word[1] + Pitch_bend[1];
	phase_accumulators[2] += frequancy_tuning_word[2];
	phase_accumulators[3] += frequancy_tuning_word[3];
	phase_accumulators[4] += frequancy_tuning_word[4];
	phase_accumulators[5] += frequancy_tuning_word[5];
	phase_accumulators[6] += frequancy_tuning_word[6];
	phase_accumulators[7] += frequancy_tuning_word[7] + Pitch_bend[7];

  
	while (phase_accumulators[0] >= max_length[0]) phase_accumulators[0] -= loop_point[0];
	while (phase_accumulators[1] >= max_length[1]) phase_accumulators[1] -= loop_point[1];
	while (phase_accumulators[2] >= max_length[2]) phase_accumulators[2] -= loop_point[2];
	while (phase_accumulators[3] >= max_length[3]) phase_accumulators[3] -= loop_point[3];
	while (phase_accumulators[4] >= max_length[4]) phase_accumulators[4] -= loop_point[4];
	while (phase_accumulators[5] >= max_length[5]) phase_accumulators[5] -= loop_point[5];
	while (phase_accumulators[6] >= max_length[6]) phase_accumulators[6] -= loop_point[6];
	if (phase_accumulators[7] >= max_length[7]) phase_accumulators[7] -= loop_point[7];
  
	int16_t wave_zero = 127 - *(wavs[0] + ((phase_accumulators[0]) >> 8));
	int16_t wave_one = 127 - *(wavs[1] + (phase_accumulators[1] >> 8));
	int16_t wave_two = 127 - *(wavs[2] + (phase_accumulators[2] >> 8));
	int16_t wave_three = 127 - *(wavs[3] + (phase_accumulators[3] >> 8));
	int16_t wave_four = 127 - *(wavs[4] + (phase_accumulators[4] >> 8));
	int16_t wave_five = 127 - *(wavs[5] + (phase_accumulators[5] >> 8));
	int16_t wave_six = 127 - *(wavs[6] + (phase_accumulators[6] >> 8));
	int16_t wave_seven = 127 - *(wavs[7] + (phase_accumulators[7] >> 8));
	REG_PIOD_ODSR = 127 + ((((wave_zero * amplitude[0]) >> 8) + ((wave_one * amplitude[1]) >> 8) + ((wave_two * amplitude[2]) >> 8) + ((wave_three * amplitude[3]) >> 8)+
					((wave_four * amplitude[4]) >> 8) + ((wave_five * amplitude[5]) >> 8) + ((wave_six * amplitude[6]) >> 8) + ((wave_seven * amplitude[7]) >> 8)) >> 2);

	//************************************************
	//  Modulation engine
	//************************************************
	//  FTW[divider] = PITCH[divider] + (int)   (((PITCH[divider]/64)*(EPCW[divider]/64)) /128)*MOD[divider];
	frequancy_tuning_word[divider] = pitch[divider] + (((pitch[divider]>>6)*(envelope_phase[divider]>>6))/128);// + Pitch_bend[divider] - 128;//MOD[divider];
	time_hz++;
}
