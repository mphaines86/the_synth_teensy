/*
 * IncFile1.h
 *
 * Created: 2/12/2016 9:55:45 PM
 *  Author: Michael Haines
 */ 


#ifndef synth_H_
#define synth_H_

#define SYNTH_VOICE_COUNT 8

#include <Arduino.h>

extern volatile uint32_t phase_accumulators[8];      //-Wave phase accumulators
extern volatile uint16_t frequancy_tuning_word[8];           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
extern volatile uint16_t amplitude[8];           //-Wave amplitudes [0-255]
extern volatile uint16_t pitch[8];          //-Voice pitch
extern volatile int modulation[8];                         //-Voice envelope modulation [0-1023 512=no mod. <512 pitch down >512 pitch up]
extern volatile uint8_t *wavs[8];                                   //-Wave table selector [address of wave in memory]
extern volatile uint16_t *envs[8];                                  //-Envelope selector [address of envelope in memory]
extern volatile uint16_t  envelope_phase[8]; //-Envelope phase accumulator
extern volatile uint16_t  env_fast_tuning_word[8];               //-Envelope speed tuning word

extern volatile uint32_t max_length[8];
extern volatile uint32_t loop_point[8];

extern volatile unsigned char divider;//-Sample rate decimator for envelope
extern volatile uint16_t time_hz;
extern volatile unsigned char tik; 
extern volatile unsigned char output_mode;

extern volatile uint16_t wave_amplitude[8];
extern volatile int16_t Pitch_bend[8];
extern volatile int noteTrigger[8];
extern volatile int noteDeath[8];
extern volatile int current_stage;
extern volatile uint16_t test_variable;

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************

void TC5_Handler();
void set_envelopes();

#endif /* INCFILE1_H_ */
