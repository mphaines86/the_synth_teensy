/*
 * IncFile1.h
 *
 * Created: 2/12/2016 9:55:45 PM
 *  Author: Michael Haines
 */


#ifndef SYNTH_H
#define SYNTH_H

#define SYNTH_VOICE_COUNT 16
#define FS 32000.0

#include <Arduino.h>

extern volatile uint32_t phase_accumulators[SYNTH_VOICE_COUNT];      //-Wave phase accumulators
extern volatile uint16_t frequancy_tuning_word[SYNTH_VOICE_COUNT];           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
extern volatile uint16_t amplitude[SYNTH_VOICE_COUNT];           //-Wave amplitudes [0-255]
extern volatile uint16_t pitch[SYNTH_VOICE_COUNT];          //-Voice pitch
extern volatile const uint8_t *wavs[SYNTH_VOICE_COUNT];                                   //-Wave table selector [address of wave in memory]

extern volatile uint32_t max_length[SYNTH_VOICE_COUNT];
extern volatile uint32_t loop_point[SYNTH_VOICE_COUNT];
extern volatile uint16_t cv_pitch[SYNTH_VOICE_COUNT];

extern volatile unsigned char divider;//-Sample rate decimator for envelope
extern volatile uint16_t time_hz;
extern volatile unsigned char tik;
extern volatile unsigned char output_mode;

extern volatile uint16_t wave_amplitude[SYNTH_VOICE_COUNT];
extern volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT];
extern volatile int noteTrigger[SYNTH_VOICE_COUNT];
extern volatile int noteDeath[SYNTH_VOICE_COUNT];
extern volatile int current_stage;
extern volatile int32_t test_variable;
extern volatile byte env_set;

extern volatile int free_notes[SYNTH_VOICE_COUNT];
extern volatile byte notes[SYNTH_VOICE_COUNT];

extern volatile uint8_t aftertouch;
extern volatile uint8_t filter_cutoff;
extern volatile uint16_t filterEnvs_amount;
extern volatile uint16_t resonantEnvs_amount;
extern volatile int8_t global_detune;

extern volatile uint8_t cv7;
extern volatile uint8_t cv70;
extern volatile uint8_t cv2;
extern volatile uint8_t cv12;
extern volatile uint16_t filter_total;

//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************
void set_envelopes();
void set_oscillators();
void set_lfo();
void TC5_Handler();
void begin();
void mTrigger(unsigned char voice,unsigned char MIDInote, uint16_t given_amplitude, int8_t detune, struct Voice * given_voice);
void note_trigger(byte channel, byte pitch, byte velocity);
void NoteRelease(byte channel, byte pitch, byte velocity);
void set_pitch(unsigned char voice, unsigned char MIDInote, int8_t detune);
void ControlChange(byte number, byte value);



#endif /* INCFILE1_H_ */
