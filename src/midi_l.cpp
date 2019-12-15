#include "midi_l.h"
#include "synth.h"
#include "Interface/interface.h"

#define NRPNCONTROLLSB 98
#define NRPNCONTROLMSB 99
#define NRPNCOARSE 6
#define NRPNFINE 38
#define MIDI_COURSE 8
#define MIDI_FINE 80
#define MIDI_STEP 120


uint8_t NRPNBit = 0;
uint8_t NRPNmsb = 0;
uint8_t NRPNlsb = 0;
uint16_t NRPNParameter = 0;


MIDI_CREATE_INSTANCE(HardwareSerial, Serial4, MIDI)

uint16_t midiCombineBytes(uint8_t first, uint8_t second)
{
    uint16_t _14bit;
    _14bit = (uint16_t)second;
    _14bit <<= 7;
    _14bit |= (uint16_t)first;
    return _14bit;
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    note_trigger(pitch, velocity);
    //Serial.println(test_variable);
    //Serial.println(pitch);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    NoteRelease(pitch);
    //Serial.println(test_variable);
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
    return;
    uint8_t paramater;
    uint8_t change = 0;

    if (number == NRPNCONTROLMSB){
        NRPNBit = 1;
        NRPNmsb = value;
    }
    else if (number == NRPNCONTROLLSB){
        NRPNlsb = value;
        // NRPNParameter = midiCombineBytes(NRPNlsb, NRPNmsb);
        NRPNParameter = NRPNmsb;
    }
    else if (number == NRPNCOARSE && NRPNBit){
        paramater = NRPNParameter - MIDI_COURSE;

        if((cpParameterList[paramater]>>9)!=value){
            cpParameterList[paramater]&=0x01fc;
            cpParameterList[paramater]=(uint16_t)value<<9;
            //change = 1;
        }

    }
    else if (number == NRPNFINE && NRPNBit){
        paramater = NRPNParameter - MIDI_COURSE;

        if(((cpParameterList[paramater]>>2)&0x7F)!=value){
            cpParameterList[paramater]&=0xFE00;
            cpParameterList[paramater]|=(uint16_t)value<<2;
            change = 1;
            NRPNBit = 0;
        }
    }
    else if (number>=MIDI_STEP){
        paramater=number-MIDI_STEP;
        uint8_t v = value>>(7-spParameterBits[paramater]);
        if(spParameterList[paramater]!=v){
            spParameterList[paramater] = v;
            change = 1;
        }
    }
    else if (number>=MIDI_COURSE){
        paramater = number - MIDI_COURSE;

        if((cpParameterList[paramater]>>1)!=value){
            cpParameterList[paramater]=value<<1;
            change=1;
        }
    }

    if(change){
        parameterChange();
    }


}

void handleAftertouch(byte channel, byte value){
    aftertouch = .4 * value;
    //Serial.println(test_variable);
}

void midi_begin() {

    /*FTM0_MODE |= FTM_MODE_WPDIS | FTM_MODE_FTMEN | FTM_MODE_INIT;
    FTM0_SC = 0;
    FTM0_CNT = 0;
    FTM0_MOD = F_BUS / (1 * 31250) - 1;
    FTM0_SC = FTM_SC_CLKS(0b01) | FTM_SC_PS(0b000) | FTM_SC_TOF | FTM_SC_TOIE;
    NVIC_ENABLE_IRQ(IRQ_FTM0);*/

    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandlePitchBend(handlePitchBend);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleAfterTouchChannel(handleAftertouch);
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void midi_read(){
    MIDI.read();
}
