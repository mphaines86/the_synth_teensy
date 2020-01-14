
#ifndef MIDI_H
#define MIDI_H

#include "main.h"
#include <MIDI.h>


void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleStop();
void handlePitchBend(byte channel, int bend);
void handleControlChange(byte channel, byte pitch, byte velocity);
void handleAftertouch(byte channel, byte value);

void midi_begin();
void midi_read();
// extern midi::MidiInterface<HardwareSerial> MIDI;

#endif