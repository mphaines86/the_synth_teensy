
#include <Interface/interface.h>
#include "main.h"
#include "Arduino.h"
#include "synth.h"
#include "system/i2c_t3.h"
#include "midi_l.h"

int current_notes = 0;
byte pitch_bend_channel[] = {0, 0, 0, 0, 0, 0, 0, 0,};

int main(void) {

    Serial.begin(9600);

    pinMode(15, OUTPUT);
    pinMode(22, OUTPUT);
    pinMode(23, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);

    pinMode(2, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(20, OUTPUT);
    pinMode(21, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(47, OUTPUT);
    pinMode(48, OUTPUT);

    pinMode(50, OUTPUT);
    pinMode(51, OUTPUT);
    pinMode(52, OUTPUT);
    pinMode(53, OUTPUT);
    pinMode(54, OUTPUT);
    pinMode(55, OUTPUT);

    pinMode(43, OUTPUT);
    pinMode(46, OUTPUT);
    pinMode(44, OUTPUT);
    pinMode(45, OUTPUT);

    pinMode(24, OUTPUT);
    pinMode(39, OUTPUT);
    pinMode(37, INPUT);
    pinMode(38, INPUT);

    test_variable = 0;
    //analogRead(23)
    synthBegin();

    synth_main();
}



/*void ftm0_isr(){
		//TC_GetStatus(TC1, 1);
    MIDI.read();
    FTM0_SC &= ~FTM_SC_TOF;

}*/



