
#include <MIDI.h>
#include "Arduino.h"
extern "C" {
  #include "synth.h"
}

#define outputSelector 9
#define CS 22
#define WR 24
#define NUM_SYNTH 8
int current_notes = 0;
//byte notes[NUM_SYNTH];
//int free_notes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,};
byte pitch_bend_channel[] = {0, 0, 0, 0, 0, 0, 0, 0,};

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handlePitchBend(byte channel, int bend);
void handleControlChange(byte channel, byte pitch, byte velocity);
void handleAftertouch(byte channel, byte value);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void setup() {

    //pinMode(13, OUTPUT);
    //digitalWrite(13, LOW);

    pinMode(A0 ,INPUT);

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
    pinMode(51, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);

    //digitalWrite(outputSelector, LOW);
    digitalWrite(CS, LOW);
    digitalWrite(WR, LOW);
    //digitalWrite(30, LOW);
    digitalWrite(31, LOW);
    digitalWrite(41, LOW);
    digitalWrite(51, LOW);

    REG_PIOD_OWER = 0xFFFF;
    REG_PIOC_OWER = 0xFFFF;

    begin();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleAfterTouchChannel(handleAftertouch);
    MIDI.begin(MIDI_CHANNEL_OMNI);

}

void loop()
{
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{

    note_trigger(channel, pitch, velocity);
	  Serial.println(test_variable);
	  //Serial.println(pitch);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    NoteRelease(channel, pitch, velocity);
    Serial.println(test_variable);
}

void handlePitchBend(byte channel, int bend){
  //int i;
  //for(i=0; i < NUM_SYNTH; i++){
    //if (pitch_bend_channel[i] == channel){
      //edgar.pitchBend(i,bend);
    //}
  //}
}

void handleControlChange(byte channel, byte number, byte value){
  ControlChange(number, value);

}

void handleAftertouch(byte channel, byte value){
    aftertouch = .4 * value;
}
