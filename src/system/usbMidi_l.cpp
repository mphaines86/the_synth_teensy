#include "usbMidi_l.h"
#include "../synth.h"
#include "../Interface/interface.h"
#include "main.h" //TODO: Remove and improve DEBUG code
#ifndef DEBUG
/*#define NRPNCONTROLLSB 98
#define NRPNCONTROLMSB 99
#define NRPNCOARSE 6
#define NRPNFINE 38
#define MIDI_COURSE 8
#define MIDI_FINE 80
#define MIDI_STEP 120*/


static uint8_t NRPNBit = 0;
static uint8_t NRPNmsb = 0;
static uint8_t NRPNlsb = 0;
static uint16_t NRPNParameter = 0;


static uint16_t midiCombineBytes(uint8_t first, uint8_t second)
{
    uint16_t _14bit;
    _14bit = (uint16_t)second;
    _14bit <<= 7;
    _14bit |= (uint16_t)first;
    return _14bit;
}

void usbMidiHandleNoteOn(byte channel, byte pitch, byte velocity)
{
    note_trigger(pitch, velocity);
    //Serial.println(test_variable);
    // Serial.println(pitch);
}

void usbMidiHandleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Do something when the note is released.
    // Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.
    NoteRelease(pitch);
    //Serial.println(test_variable);
}

void usbMidiHandleStop(){
    // Serial.println("Stop!");
    NoteRelease();
}

void usbMidiHandlePitchBend(byte channel, int bend){
    //int i;
    //for(i=0; i < NUM_SYNTH; i++){
    //if (pitch_bend_channel[i] == channel){
    //edgar.pitchBend(i,bend);
    //}
    //}
}

void usbMidiHandleControlChange(byte channel, byte pitch, byte velocity){
    // Serial.println(number);
    /*uint8_t paramater;
    uint8_t change = 0;

    if (pitch == NRPNCONTROLMSB){
        NRPNBit = 1;
        NRPNmsb = velocity;
    }
    else if (pitch == NRPNCONTROLLSB){
        NRPNlsb = velocity;
        // NRPNParameter = midiCombineBytes(NRPNlsb, NRPNmsb);
        NRPNParameter = NRPNmsb;
    }
    else if (pitch == NRPNCOARSE && NRPNBit){
        paramater = NRPNParameter - MIDI_COURSE;

        if((cpParameterList[paramater]>>9)!=velocity){
            cpParameterList[paramater]&=0x01fc;
            cpParameterList[paramater]=(uint16_t)velocity<<9;
            //change = 1;
        }

    }
    else if (pitch == NRPNFINE && NRPNBit){
        paramater = NRPNParameter - MIDI_COURSE;

        if(((cpParameterList[paramater]>>2)&0x7F)!=velocity){
            cpParameterList[paramater]&=0xFE00;
            cpParameterList[paramater]|=(uint16_t)velocity<<2;
            change = 1;
            NRPNBit = 0;
        }
    }
    else if (pitch>=MIDI_STEP){
        paramater=pitch-MIDI_STEP;
        uint8_t v = velocity>>(7-spParameterBits[paramater]);
        if(spParameterList[paramater]!=v){
            spParameterList[paramater] = v;
            change = 1;
        }
    }
    else if (pitch>=MIDI_COURSE){
        paramater = pitch - MIDI_COURSE;

        if((cpParameterList[paramater]>>1)!=velocity){
            cpParameterList[paramater]=velocity<<1;
            change=1;
        }
    }

    if(change){
        synthParameterChange();
    }*/


}

void usbMidiHandleAftertouch(byte channel, byte value){
    aftertouch = .4 * value;
    //Serial.println(test_variable);
}

void usbMidiBegin() {

    /*FTM0_MODE |= FTM_MODE_WPDIS | FTM_MODE_FTMEN | FTM_MODE_INIT;
    FTM0_SC = 0;
    FTM0_CNT = 0;
    FTM0_MOD = F_BUS / (1 * 31250) - 1;
    FTM0_SC = FTM_SC_CLKS(0b01) | FTM_SC_PS(0b000) | FTM_SC_TOF | FTM_SC_TOIE;
    NVIC_ENABLE_IRQ(IRQ_FTM0);*/

    usbMIDI.setHandleNoteOn(usbMidiHandleNoteOn);
    usbMIDI.setHandleNoteOff(usbMidiHandleNoteOff);
    usbMIDI.setHandlePitchChange(usbMidiHandlePitchBend);
    usbMIDI.setHandleControlChange(usbMidiHandleControlChange);
    usbMIDI.setHandleAfterTouchChannel(usbMidiHandleAftertouch);
    usbMIDI.setHandleStop(usbMidiHandleStop);
    usbMIDI.setHandleSystemReset(usbMidiHandleStop);
    //usbMIDI.begin(MIDI_CHANNEL_OMNI);
}

void usbMidiRead(){
    usbMIDI.read();
}

#endif