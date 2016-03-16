/*
 * IncFile1.h
 *
 * Created: 2/12/2016 9:55:45 PM
 *  Author: Michael Haines
 */


#ifndef SYNTH_H
#define SYNTH_H

#define SYNTH_VOICE_COUNT 16

#include <Arduino.h>

extern volatile uint32_t phase_accumulators[SYNTH_VOICE_COUNT];      //-Wave phase accumulators
extern volatile uint16_t frequancy_tuning_word[SYNTH_VOICE_COUNT];           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
extern volatile uint16_t amplitude[SYNTH_VOICE_COUNT];           //-Wave amplitudes [0-255]
extern volatile uint16_t pitch[SYNTH_VOICE_COUNT];          //-Voice pitch
extern volatile const uint8_t *wavs[SYNTH_VOICE_COUNT];                                   //-Wave table selector [address of wave in memory]

extern volatile uint32_t max_length[SYNTH_VOICE_COUNT];
extern volatile uint32_t loop_point[SYNTH_VOICE_COUNT];

extern volatile unsigned char divider;//-Sample rate decimator for envelope
extern volatile uint16_t time_hz;
extern volatile unsigned char tik;
extern volatile unsigned char output_mode;

extern volatile uint16_t wave_amplitude[SYNTH_VOICE_COUNT];
extern volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT];
extern volatile int noteTrigger[SYNTH_VOICE_COUNT];
extern volatile int noteDeath[SYNTH_VOICE_COUNT];
extern volatile int current_stage;
extern volatile uint16_t test_variable;

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************
void set_envelopes();
void set_oscillators();
void set_lfo();
void TC5_Handler();


#endif /* INCFILE1_H_ */
