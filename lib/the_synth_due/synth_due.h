#ifndef SYNTH_DUE
#define SYNTH_DUE
//*************************************************************************************
//  Arduino synth V4.1
//  Optimized audio driver, modulation engine, envelope engine.
//
//  Dzl/Illutron 2014
//
//*************************************************************************************

#include "tables_due.h"
#include "synth.h"
#include "pitch_tables.h"

#define SINE     0
#define TRIANGLE 1
#define SQUARE   2
#define SAW      3
#define RAMP     4
#define NOISE    5
#define CUSTOM   6

#define ENVELOPE0 0
#define ENVELOPE1 1
#define ENVELOPE2 2
#define ENVELOPE3 3

#define FS 32000.0                                              //-Sample rate (NOTE: must match tables.h)

class synth
{
private:

public:


  synth()
  {
    extern void set_envelopes();
  }

  //*********************************************************************
  //  Startup default
  //*********************************************************************

  void begin()
  {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)TC5_IRQn);
        TC_Configure(TC1, 2, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/FS; //128 because we selected TIMER_CLOCK4 above
        //TC_SetRA(TC1, 2, rc/2); //50% high, 50% low
        TC_SetRC(TC1, 2, rc);
        TC_Start(TC1, 2);
        TC1->TC_CHANNEL[2].TC_IER=TC_IER_CPCS;
        TC1->TC_CHANNEL[2].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(TC5_IRQn);
		      //set_envelopes();
  }

  //*********************************************************************
  //  Startup fancy selecting varoius output modes
  //*********************************************************************

  void begin(unsigned char d)
  {


  }

  //*********************************************************************
  //  Timing/sequencing functions
  //*********************************************************************

  unsigned char synthTick(void)
  {
    if(tik)
    {
      tik=0;
      return 1;  //-True every 4 samples
    }
    return 0;
  }

  unsigned char voiceFree(unsigned char voice)
  {
    return 1;
  }


  //*********************************************************************
  //  Setup all voice parameters in MIDI range
  //  voice[0-3],wave[0-6],pitch[0-127],envelope[0-4],length[0-127],mod[0-127:64=no mod]
  //*********************************************************************

  void setupVoice(unsigned char voice,const uint8_t *wave, unsigned char pitch, unsigned char env, unsigned char length, unsigned int mod)
  {
    setWave(voice,wave);
    setPitch(voice,pitch, 0);
    setEnvelope(voice,env);
    setMod(voice,mod);
  }

  //*********************************************************************
  //  Setup wave [0-6]
  //*********************************************************************

  void setWave(unsigned char voice,const uint8_t *wave){

	  wavs[voice] = wave;

  }
  //*********************************************************************
  //  Setup Pitch [0-127]
  //*********************************************************************

  void setPitch(unsigned char voice,unsigned char MIDInote, uint8_t detune)
  {
    /*uint8_t octave = MIDInote / 12;
    uint8_t pitch_index = (MIDInote - octave * 12) * 20;
    uint16_t pre_pitching = One_Octave_Pitches[pitch_index + detune];
    pitch[voice] = (pre_pitching>>(10 - octave));*/
    /*switch (octave) {
      case 0:
        pitch[voice] = pre_pitching>>10;
        break;
      case 1:
        pitch[voice] = pre_pitching>>9;
        break;
      case 2:
        pitch[voice] = pre_pitching>>8;
        break;
      case 3:
        pitch[voice] = pre_pitching>>7;
        break;
      case 4:
        pitch[voice] = pre_pitching>>6;
        break;
      case 5:
        pitch[voice] = pre_pitching>>5;
        break;
      case 6:
        pitch[voice] = pre_pitching>>4;
        break;
      case 7:
        pitch[voice] = pre_pitching>>3;
        break;
      case 8:
        pitch[voice] = pre_pitching>>2;
        break;
      case 9:
        pitch[voice] = pre_pitching>>1;
        break;
      case 10:
        pitch[voice] = pre_pitching;
        break;
    }*/
    uint16_t cv = 240 * MIDInote + detune * 12;
    uint8_t octave = cv / 2880;
    uint8_t pitch_index = (cv * 240) / 2880 - octave * 240;
    //test_variable = One_Octave_Pitches[pitch_index];
    uint16_t pre_pitching = One_Octave_Pitches[pitch_index];
    pitch[voice] = (pre_pitching>>(10 - octave));

  }


  //*********************************************************************
  //  Setup Envelope [0-4]
  //*********************************************************************

  void setEnvelope(unsigned char voice, unsigned char env)
  {

  }

  //*********************************************************************
  //  Setup Length [0-128]
  //*********************************************************************



  //*********************************************************************
  //  Setup mod
  //*********************************************************************

  void setMod(unsigned char voice,unsigned char mod)
  {

  }

  //*********************************************************************
  //  Midi trigger
  //*********************************************************************

  void mTrigger(unsigned char voice,unsigned char MIDInote, uint16_t amplitude, uint8_t detune, struct Voice * given_voice)
  {
		/*if (MIDInote <= 36)
		{
			wavs[voice] = piano_C3.wave;
			max_length[voice] = piano_C3.max_length;
			loop_point[voice] = piano_C3.loop_point;
			//pitch[voice]=*(&PITCHS[MIDInote - piano_C3.pitch_from_C5]);
      setPitch(voice,MIDInote - piano_C3.pitch_from_C5, 0 );
		}
		if (MIDInote > 36 && MIDInote <= 55)
		{
			wavs[voice] = piano_C5.wave;
			max_length[voice] = piano_C5.max_length;
			loop_point[voice] = piano_C5.loop_point;
			//pitch[voice]=*(&PITCHS[MIDInote - piano_C5.pitch_from_C5]);
      setPitch(voice,MIDInote - piano_C5.pitch_from_C5, 0 );
		}
		if (MIDInote > 55)
		{
			wavs[voice] = piano_C6.wave;
			max_length[voice] = piano_C6.max_length;
			loop_point[voice] = piano_C6.loop_point;
			//pitch[voice]=*(&PITCHS[MIDInote - piano_C6.pitch_from_C5]);
      setPitch(voice,MIDInote - piano_C6.pitch_from_C5, 0 );
		}*/

    wavs[voice] = given_voice->wave;
    max_length[voice] = given_voice->max_length;
    loop_point[voice] = given_voice->loop_point;
    setPitch(voice, MIDInote - given_voice->pitch_from_C5, detune);

		phase_accumulators[voice] = 0;
		wave_amplitude[voice] = amplitude;
		//frequancy_tuning_word[divider] = pitch[voice];
		noteTrigger[voice] = 1;
  }

  void noteOff(unsigned char voice){
	noteDeath[voice] = 1;
    amplitude[voice] = 0;
  }

  void pitchBend(unsigned char voice, int value){
    Pitch_bend[voice] = value/32;
  }

  //*********************************************************************
  //  Set frequency direct
  //*********************************************************************

  void setFrequency(unsigned char voice,float f)
  {
    pitch[voice]=f/(FS/65535.0);

  }

  //*********************************************************************
  //  Set time
  //*********************************************************************



  //*********************************************************************
  //  Simple trigger
  //*********************************************************************

  void trigger(unsigned char voice)
  {
    frequancy_tuning_word[voice]=pitch[voice];
    //    FTW[voice]=PITCH[voice]+(PITCH[voice]*(EPCW[voice]/(32767.5*128.0  ))*((int)MOD[voice]-512));
  }

  //*********************************************************************
  //  Suspend/resume synth
  //*********************************************************************

  /*void suspend()
  {
    CLR(TIMSK1, OCIE1A);                            //-Stop audio interrupt
  }
  void resume()
  {
    SET(TIMSK1, OCIE1A);                            //-Start audio interrupt
  }*/

};

#endif
