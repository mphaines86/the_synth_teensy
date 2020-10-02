
#ifndef usbMIDI_H
#define usbMIDI_H

// #include "usbMIDI.h"
#include <cstddef>
#include <cstdint>

void usbMidiHandleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity);
void usbMidiHandleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity);
void usbMidiHandleStop();
void usbMidiHandlePitchBend(uint8_t channel, int bend);
void usbMidiHandleControlChange(uint8_t channel, uint8_t pitch, uint8_t velocity);
void usbMidiHandleAftertouch(uint8_t channel, uint8_t value);

void usbMidiBegin();
void usbMidiRead();
// extern midi::MidiInterface<HardwareSerial> MIDI;

#endif