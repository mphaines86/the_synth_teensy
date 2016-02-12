#include "synth_due.h"
#include <MIDI.h>

#define outputSelector 9
#define CS 22
#define WR 24

int current_notes = 0;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

synth edgar;    //-Make a synth

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  pinMode(CS, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(outputSelector, OUTPUT);

  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  digitalWrite(outputSelector, LOW);
  digitalWrite(CS, LOW);
  digitalWrite(WR, LOW);

  REG_PIOD_OWER = 0xFFFF;  

  edgar.begin();                                   //-Start it up
  edgar.setupVoice(0, 2, 60, ENVELOPE3, 127, 64); //-Set up voice 0
  edgar.setupVoice(1, 2, 60, ENVELOPE3, 127, 64);
  edgar.setupVoice(2, 5, 60, ENVELOPE1, 127, 64);
  edgar.setupVoice(3, 3, 60, ENVELOPE0, 127, 64);
  edgar.setupVoice(4, 3, 60, ENVELOPE0, 127, 64);
  edgar.setupVoice(5, 3, 60, ENVELOPE0, 127, 64);
  edgar.setupVoice(6, 3, 60, ENVELOPE0, 127, 64);
  edgar.setupVoice(7, 2, 60, ENVELOPE0, 127, 64);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.begin(MIDI_CHANNEL_OMNI);

}

void loop()
{
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    edgar.mTrigger(channel - 1, pitch, velocity * 2);

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
      Serial.println(PCW[0]);
      edgar.noteOff(channel - 1);
      
}

void handlePitchBend(byte channel, int bend){
  edgar.pitchBend(channel - 1,bend);
  
}

