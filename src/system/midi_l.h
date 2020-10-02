
#ifndef MIDI_H
#define MIDI_H

#include "main.h"
#include <MIDI.h>


void midiHandleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
void midiHandleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
void midiHandleStop();
void midiHandlePitchBend(uint8_t channel, int bend);
void midiHandleControlChange(uint8_t channel, uint8_t pitch, uint8_t velocity);
void midiHandleAftertouch(uint8_t channel, uint8_t value);

void midiBegin();
void midiRead();
// extern midi::MidiInterface<HardwareSerial> MIDI;

#endif