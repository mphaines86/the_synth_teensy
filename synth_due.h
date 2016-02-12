#ifndef _SYNTH
#define _SYNTH
//*************************************************************************************
//  Arduino synth V4.1
//  Optimized audio driver, modulation engine, envelope engine.
//
//  Dzl/Illutron 2014
//
//*************************************************************************************

#include "tables_due.h"
#define DIFF 1
#define CHA 2
#define CHB 3

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

#define SET(x,y) (x |=(1<<y))		        		//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       			// |
#define CHK(x,y) (x & (1<<y))           			// |
#define TOG(x,y) (x^=(1<<y))            			//-+

volatile uint16_t PCW[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};      //-Wave phase accumolators
volatile uint16_t FTW[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};           //-Wave frequency tuning words 200, 200, 300, 400, 200, 200, 300, 400
volatile uint16_t AMP[8] = {
  0, 0, 0, 0, 0, 0, 0, 0};           //-Wave amplitudes [0-255]
volatile uint16_t PITCH[8] = {
  100, 500, 500, 500, 100, 500, 500, 500};          //-Voice pitch
volatile int MOD[8] = {
  20, 0, 64, 127, 20, 0, 64, 127};                         //-Voice envelope modulation [0-1023 512=no mod. <512 pitch down >512 pitch up]
volatile uint8_t *wavs[8];                                   //-Wave table selector [address of wave in memory]
volatile uint16_t *envs[8];                                  //-Envelopte selector [address of envelope in memory]
volatile uint16_t  EPCW[8] = {
  0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000}; //-Envelope phase accumolator
volatile uint16_t  EFTW[8] = {
  20, 10, 10, 10, 10, 10, 10, 10};               //-Envelope speed tuning word

volatile uint32_t max_length[8] = {
  65536, 65536, 268435, 65536, 65536, 65536, 65536, 65536,
};
volatile unsigned char divider = 0;//-Sample rate decimator for envelope
volatile uint16_t tim = 0;
volatile unsigned char tik = 0; 
volatile unsigned char output_mode;

volatile uint8_t wave_amplitude[8] = {
  0, 0, 0, 0, 0, 0, 0, 0,
};
volatile uint16_t Pitch_bend[8] = {
  0, 0, 0, 0, 0, 0, 0, 0,};


//*********************************************************************************************
//  Audio driver interrupt
//*********************************************************************************************

void TC5_Handler()
{

  TC_GetStatus(TC1, 2);
  //-------------------------------
  // Time division
  //-------------------------------
  divider++;
  if(!(divider&=0x07))
    tik=1;

  //-------------------------------
  // Volume envelope generator
  //-------------------------------

  if (!(EPCW[divider]&0x80)){
    EPCW[divider] +=EFTW[divider];
    AMP[divider] = (*(envs[divider] + (EPCW[divider]>>8)))&(wave_amplitude[divider]);
    
    //AMP[divider] = pgm_read_byte(envs[divider] + (((unsigned char*)&(EPCW[divider]+=EFTW[divider]))[1]));
  }
  else
    AMP[divider] = 0;

  //-------------------------------
  //  Synthesizer/audio mixer
  //-------------------------------
  

  PCW[0] += FTW[0] + Pitch_bend[0];
  PCW[1] += FTW[1] + Pitch_bend[1];
  PCW[2] += FTW[2];
  PCW[3] += FTW[3];
  PCW[4] += FTW[4];
  PCW[5] += FTW[5];
  PCW[6] += FTW[6];
  PCW[7] += FTW[7] + Pitch_bend[7];

  
  while (PCW[0] >= max_length[0]) PCW[0] -= max_length[0];
  while (PCW[1] >= max_length[0]) PCW[1] -= max_length[1];
  while (PCW[2] >= max_length[0]) PCW[2] -= max_length[2];
  while (PCW[3] >= max_length[0]) PCW[3] -= max_length[3];
  while (PCW[4] >= max_length[0]) PCW[4] -= max_length[4];
  while (PCW[5] >= max_length[0]) PCW[5] -= max_length[5];
  while (PCW[6] >= max_length[0]) PCW[6] -= max_length[6];
  while (PCW[7] >= max_length[0]) PCW[7] -= max_length[7];
  
  uint16_t wave_zero = *(wavs[0] + ((PCW[0]) >> 8));
  uint16_t wave_one = *(wavs[1] + (PCW[1] >> 8));
  uint16_t wave_two = *(wavs[2] + (PCW[2] >> 8));
  uint16_t wave_three = *(wavs[3] + (PCW[3] >> 8));
  uint16_t wave_four = *(wavs[4] + (PCW[4] >> 8));
  uint16_t wave_five = *(wavs[5] + (PCW[5] >> 8));
  uint16_t wave_six = *(wavs[6] + (PCW[6] >> 8));
  uint16_t wave_seven = *(wavs[7] + (PCW[7] >> 8));
  REG_PIOD_ODSR = (((wave_zero * AMP[0]) >> 8) + ((wave_one * AMP[1]) >> 8) + ((wave_two * AMP[2]) >> 8) + ((wave_three * AMP[3]) >> 8)+
                  ((wave_four * AMP[4]) >> 8) + ((wave_five * AMP[5]) >> 8) + ((wave_six * AMP[6]) >> 8) + ((wave_seven * AMP[7]) >> 8)) >> 3;

  //************************************************
  //  Modulation engine
  //************************************************
  //  FTW[divider] = PITCH[divider] + (int)   (((PITCH[divider]/64)*(EPCW[divider]/64)) /128)*MOD[divider];
  FTW[divider] = PITCH[divider] + (((PITCH[divider]>>6)*(EPCW[divider]>>6))/128);// + Pitch_bend[divider] - 128;//MOD[divider];
	tim++;
}

class synth
{
private:
  
public:


  synth()
  {
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
        //TC_SetRA(tc, channel, rc/2); //50% high, 50% low
        TC_SetRC(TC1, 2, rc);
        TC_Start(TC1, 2);
        TC1->TC_CHANNEL[2].TC_IER=TC_IER_CPCS;
        TC1->TC_CHANNEL[2].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(TC5_IRQn);
  
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
    if (!(((unsigned char*)&EPCW[voice])[1]&0x80))
      return 0;
    return 1;
  }


  //*********************************************************************
  //  Setup all voice parameters in MIDI range
  //  voice[0-3],wave[0-6],pitch[0-127],envelope[0-4],length[0-127],mod[0-127:64=no mod]
  //*********************************************************************

  void setupVoice(unsigned char voice, unsigned char wave, unsigned char pitch, unsigned char env, unsigned char length, unsigned int mod)
  {
    setWave(voice,wave);
    setPitch(voice,pitch);
    setEnvelope(voice,env);
    setLength(voice,length);
    setMod(voice,mod);
  }

  //*********************************************************************
  //  Setup wave [0-6]
  //*********************************************************************

  void setWave(unsigned char voice, unsigned char wave)
  {
    switch (wave)
    {
    case TRIANGLE:
      wavs[voice] = TriangleTable;
      break;
    case SQUARE:
      wavs[voice] = SquareTable;
      break;
    case SAW:
      wavs[voice] = SawTable;
      break;
    case RAMP:
      wavs[voice] = RampTable;
      break;
    case NOISE:
      wavs[voice] = sounddata_data;
      break;
    case CUSTOM:
      wavs[voice] = sounddata_data;
    default:
      wavs[voice] = SinTable;
      break;
    }
  }
  //*********************************************************************
  //  Setup Pitch [0-127]
  //*********************************************************************

  void setPitch(unsigned char voice,unsigned char MIDInote)
  {
    PITCH[voice]=PITCHS[MIDInote];
  }

  //*********************************************************************
  //  Setup Envelope [0-4]
  //*********************************************************************

  void setEnvelope(unsigned char voice, unsigned char env)
  {
    switch (env)
    {
    case 1:
      envs[voice] = Env0;
      break;
    case 2:
      envs[voice] = Env1;
      break;
    case 3:
      envs[voice] = Env2;
      break;
    case 4:
      envs[voice] = Env3;
      break;
    default:
      envs[voice] = Env0;
      break;
    }
  }

  //*********************************************************************
  //  Setup Length [0-128]
  //*********************************************************************

  void setLength(unsigned char voice,unsigned char length)
  {
    EFTW[voice]=EFTWS[length];
  }

  //*********************************************************************
  //  Setup mod
  //*********************************************************************

  void setMod(unsigned char voice,unsigned char mod)
  {
    //    MOD[voice]=(unsigned int)mod*8;//0-1023 512=no mod
    MOD[voice]=(int)mod-64;//0-1023 512=no mod
  }

  //*********************************************************************
  //  Midi trigger
  //*********************************************************************

  void mTrigger(unsigned char voice,unsigned char MIDInote, unsigned char amplitude)
  {
    PITCH[voice]=*(&PITCHS[MIDInote]);
    EPCW[voice]=0;
    wave_amplitude[voice] = amplitude;
    FTW[divider] = PITCH[voice] + (int)   (((PITCH[voice]>>6)*(EPCW[voice]>>6))/128);//*MOD[voice];
  }

  void noteOff(unsigned char voice){
    EPCW[voice]=0x80;
    AMP[voice] = 0;
  }

  void pitchBend(unsigned char voice, int value){
    Pitch_bend[voice] = value/32;
  }

  //*********************************************************************
  //  Set frequency direct
  //*********************************************************************

  void setFrequency(unsigned char voice,float f)
  {
    PITCH[voice]=f/(FS/65535.0);

  }

  //*********************************************************************
  //  Set time
  //*********************************************************************

  void setTime(unsigned char voice,float t)
  {
    EFTW[voice]=(1.0/t)/(FS/(32767.5*10.0));//[s];
  }

  //*********************************************************************
  //  Simple trigger
  //*********************************************************************

  void trigger(unsigned char voice)
  {
    EPCW[voice]=0;
    FTW[voice]=PITCH[voice];
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













