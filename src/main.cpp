
#include <MIDI.h>
#include "Arduino.h"
extern "C" {
  #include "synth.h"

}

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

    pinMode(8 ,INPUT);
    pinMode(7 ,INPUT);

    Serial.begin(115200);
    pinMode(CS, OUTPUT);
    pinMode(WR, OUTPUT);
    //pinMode(outputSelector, OUTPUT);

    for(int i = 25; i < 42; i++){
        pinMode(i, OUTPUT);
    }

    pinMode(12, OUTPUT);
    pinMode(9, OUTPUT);

    for(int i = 42; i<=50; i++){
      pinMode(i, OUTPUT);
    }

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
    //digitalWrite(12, HIGH);

    //REG_PIOD_OWER = 0xFFFF;
    //REG_PIOC_OWER = 0xFFFF;
    begin();

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleAfterTouchChannel(handleAftertouch);
    MIDI.begin(MIDI_CHANNEL_OMNI);

    pmc_enable_periph_clk((uint32_t)TC4_IRQn);
    TC_Configure(TC1, 1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    uint32_t rc = VARIANT_MCK/128/500; //128 because we selected TIMER_CLOCK4 above
    //TC_SetRA(TC1, 2, rc/2); //50% high, 50% low
    TC_SetRC(TC1, 1, rc);
    TC_Start(TC1, 1);
    TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
    TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
    NVIC_EnableIRQ(TC4_IRQn);
    //delay(3000);

}

void TC4_Handler(){
		TC_GetStatus(TC1, 1);
    MIDI.read();

}

void loop()
{
    //while(1){
    //Serial.println(test_variable);
    interfaceCheck();

      //}
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
