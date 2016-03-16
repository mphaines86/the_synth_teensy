#include <synth_due.h>
#include <MIDI.h>
#include "Arduino.h"

#define outputSelector 9
#define CS 22
#define WR 24
#define NUM_SYNTH 16
int current_notes = 0;
byte notes[NUM_SYNTH];
int free_notes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,};

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handlePitchBend(byte channel, int bend);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

synth edgar;    //-Make a synth

void setup() {

    //pinMode(13, OUTPUT);
    //digitalWrite(13, LOW);

    Serial.begin(115200);
    pinMode(CS, OUTPUT);
    pinMode(WR, OUTPUT);
    //pinMode(outputSelector, OUTPUT);

    for(int i = 25; i < 42; i++){
        pinMode(i, OUTPUT);
    }
    //pinMode(25, OUTPUT);
    //pinMode(26, OUTPUT);
    //pinMode(27, OUTPUT);
    //pinMode(28, OUTPUT);
    //pinMode(29, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);

    //digitalWrite(outputSelector, LOW);
    digitalWrite(CS, LOW);
    digitalWrite(WR, LOW);
    //digitalWrite(30, LOW);
    //digitalWrite(31, LOW);
    //digitalWrite(41, LOW)

    REG_PIOD_OWER = 0xFFFF;
    REG_PIOC_OWER = 0xFFFF;

    edgar.begin();
    for(byte i=0; i<NUM_SYNTH; i+=2){
	     edgar.setupVoice(i, EuroBell1.wave, 60, ENVELOPE1, 127, 64); //-Set up voice 0
    }
    for(byte i=1; i<NUM_SYNTH; i+=2){
      edgar.setupVoice(i, EuroBell2.wave, 60, ENVELOPE1, 127, 64); //-Set up voice 0
    }

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

		for(int i= 0; i < NUM_SYNTH; i++){
 			  if(free_notes[i] > -1){
				    notes[i] = pitch;
            notes[i+1] = pitch; // Second Oscillator
				    edgar.mTrigger(i, pitch, velocity * 516, 0, &EuroBell2);
            edgar.mTrigger(i + 1, pitch, velocity * 516, 2, &EuroBell1); // Second Oscillator
				    free_notes[i] = -1;
            free_notes[i + 1] = -1; // Second Oscillator
				    break;
			  }
		}
	  //Serial.println(test_variable);
	  //Serial.println(current_stage);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
     //Serial.println(test_variable);
	  //Serial.println(current_stage);
	  for(int i= 0; i < NUM_SYNTH; i++){
		    if(notes[i] == pitch){
			      edgar.noteOff(i);
			      free_notes[i] = i;
			//trigger_note[pitch] = -1;
		    }
	  }
}

void handlePitchBend(byte channel, int bend){
    edgar.pitchBend(channel - 1,bend);

}
