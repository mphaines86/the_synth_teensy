/*
 * IncFile1.h
 *
 * Created: 2/12/2016 9:55:45 PM
 *  Author: Michael Haines
 */


#ifndef SYNTH_H
#define SYNTH_H

#define SYNTH_VOICE_COUNT 4
#define FS 32000

#include <Arduino.h>


extern volatile uint16_t amplitude[SYNTH_VOICE_COUNT];           //-Wave amplitudes [0-255]
extern volatile uint16_t current_note[SYNTH_VOICE_COUNT];

extern volatile unsigned char divider;//-Sample rate decimator for envelope
extern volatile uint64_t tik;

extern volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT];
extern volatile int noteTrigger[SYNTH_VOICE_COUNT];
extern volatile int noteDeath[SYNTH_VOICE_COUNT];
extern volatile int current_stage;
extern volatile int32_t test_variable;

extern int free_notes[SYNTH_VOICE_COUNT];
extern byte notes[SYNTH_VOICE_COUNT];

extern uint8_t aftertouch;

extern volatile uint16_t filter_total;

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************

void synthBegin();
void note_trigger(byte given_pitch, byte velocity);
void NoteRelease(byte given_pitch);
void synthParameterChange();
void synthWaveChange(uint8_t wave, uint8_t voice, uint8_t oscillator, uint8_t lowNote, uint8_t highNote);

void set_envelopes();
void set_oscillators();
void set_lfo();



#endif /* INCFILE1_H_ */
