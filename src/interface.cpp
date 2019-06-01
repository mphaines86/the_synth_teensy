#include "interface.h"
#include "system/HD44780.h"
#include <synth.h>
#include <util/delay.h>

#define TIME_BUFFER 100
#define DEBOUNCE_MAX 50
#define PARAMETER_PAGES 8
#define POT_TIME 250
#define POT_THRESHOLD 1000
#define NUM_OF_POTS 8
#define NUM_OF_SAMPLES 20

uint16_t cpParameterList[cpCount];
uint16_t spParameterList[spCount];
uint8_t analogBusy = 0;
uint8_t updatePage = 0;

uint8_t spParameterBits[spCount] = {
        4, // wave A
        4, // wave B
        1, // sync
        1, // fltr
        1, //
        1, // Amp
        1, //
        1, // Aux
        1, //
        2, // LFO A wave
        1, // LFO A
        1, // LFO A
        1, // LFO A
        2, // LFO B
        1, // LFO B
        1, // LFO B
        1, // LFO B

};

static struct {
    uint16_t pot_samples[NUM_OF_POTS][NUM_OF_SAMPLES];
    uint64_t pot_time[NUM_OF_POTS];
    uint8_t increament_potentiometer;
    uint16_t pot_value[NUM_OF_POTS];
    uint16_t current_sample[NUM_OF_POTS];
    uint8_t potAdress[NUM_OF_POTS];

    int8_t integrator[2];
    uint8_t page;
    uint32_t lastPressTime[2];
    uint8_t lastButtonPress[2];

    uint8_t temp_parameter;

} interface;

enum interfaceParamType_e {
  parameterNone=0, parameterCont, parameterStep
};

struct interfaceParam_s{
  enum interfaceParamType_e type;
  uint8_t number;
  const char * shortName;
  const char * longName;
  const char * values[8];
};

const struct interfaceParam_s interfaceParameters[PARAMETER_PAGES][8] = {
    {
        {.type=parameterCont, .number=oscAfreq, .shortName="AFrq", .longName="Osc A Frequancy"},
        {.type=parameterCont, .number=oscACrse, .shortName="ACrs", .longName="Osc A Coarse"},
        {.type=parameterCont, .number=oscAVol, .shortName="AVol", .longName="Osc A Volume"},
        {.type=parameterCont, .number=oscAMod, .shortName="AMod", .longName="Osc A Mod"},
        {.type=parameterStep, .number=spOscSync, .shortName="Sync", .longName="Osc Sync"},
        {.type=parameterStep, .number=spOscAWave, .shortName="Awav", .longName="Osc A Waveform"},
        {.type=parameterNone},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=oscBfreq, .shortName="BFrq", .longName="Osc B Frequancy"},
        {.type=parameterCont, .number=oscBCrse, .shortName="BCrs", .longName="Osc B Coarse"},
        {.type=parameterCont, .number=oscBVol, .shortName="BVol", .longName="Osc B Volume"},
        {.type=parameterCont, .number=oscBMod, .shortName="BMod", .longName="Osc B Mod"},
        {.type=parameterCont, .number=oscFMMod, .shortName="Fmod", .longName="Osc B FM Mod"},
        {.type=parameterStep, .number=spOscBWave,.shortName="Bwav", .longName="Osc B Waveform"},
        {.type=parameterNone},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=fltrCutoff, .shortName="FCut", .longName="Filter Cutoff"},
        {.type=parameterCont, .number=fltrResonance, .shortName="FRes", .longName="Filter Resonance"},
        {.type=parameterCont, .number=fltrEnvMnt, .shortName="FEnv", .longName="Filter Env Amount"},
        {.type=parameterCont, .number=fltrMod, .shortName="FMod", .longName="Filter Modulation"},
        {.type=parameterNone},
        {.type=parameterNone},
        {.type=parameterNone},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=fltrAtt, .shortName="FAtk", .longName="Filter Attack"},
        {.type=parameterCont, .number=fltrDec, .shortName="FDec", .longName="Filter Decay"},
        {.type=parameterCont, .number=fltrSus, .shortName="FSus", .longName="Filter Sustain"},
        {.type=parameterCont, .number=fltrRel, .shortName="FRel", .longName="Filter Release"},
        {.type=parameterCont, .number=fltrAmp, .shortName="FAmp", .longName="Filter Env Amp"},
        {.type=parameterStep, .number=spFltrEnvTrig, .shortName="FTrg", .longName="Filter Env Trig"},
        {.type=parameterStep, .number=spFltrEnvSpd, .shortName="FSpd", .longName="Filter Env Speed"},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=AmpAtt, .shortName="ACut", .longName="Ampitude Attack"},
        {.type=parameterCont, .number=AmpDec, .shortName="ADec", .longName="Amplitude Decay"},
        {.type=parameterCont, .number=AmpSus, .shortName="ASus", .longName="Amplitude Sustain"},
        {.type=parameterCont, .number=AmpRel, .shortName="ARel", .longName="Amplitude Release"},
        {.type=parameterCont, .number=AmpAmp, .shortName="AAmp", .longName="Amplitude Env Amp"},
        {.type=parameterStep, .number=spAmpEnvTrig, .shortName="ATrg", .longName="Amplitude Env Trig"},
        {.type=parameterStep, .number=spAmpEnvSpd, .shortName="ASpd", .longName="Amplitude Env Speed"},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=AuxAtt, .shortName="XAtk", .longName="Auxiliry Attack"},
        {.type=parameterCont, .number=AuxDec, .shortName="XDec", .longName="Auxiliry Decay"},
        {.type=parameterCont, .number=AuxSus, .shortName="XSus", .longName="Auxiliry Sustain"},
        {.type=parameterCont, .number=AuxRel, .shortName="XRel", .longName="Auxiliry Release"},
        {.type=parameterCont, .number=AuxAmp, .shortName="XAmp", .longName="Auxiliry Env Amp"},
        {.type=parameterStep, .number=spAuxEnvTrig, .shortName="XTrg", .longName="Auxiliry Env Trig"},
        {.type=parameterStep, .number=spAuxEnvSpd, .shortName="XSpd", .longName="Auxiliry Env Speed"},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=lfoAPitch, .shortName="AAmp", .longName="LFO A Amplitude"},
        {.type=parameterCont, .number=lfoARate, .shortName="ARte", .longName="LFO A Rate"},
        {.type=parameterStep, .number=spLfoAShape, .shortName="AShp", .longName="LFO A Shape"},
        {.type=parameterStep, .number=spLfoAspeed, .shortName="Aspd", .longName="LFO A Speed"},
        {.type=parameterStep, .number=spLfoATrk, .shortName="ATrk", .longName="LFO A Tracking"},
        {.type=parameterStep, .number=spLfoATrig, .shortName="ATrg", .longName="LFO A Trig"},
        {.type=parameterCont, .number=rampAmount, .shortName="RAmt", .longName="Ramp Amplitude"},
        {.type=parameterCont, .number=rampRate, .shortName="RRte", .longName="Ramp Rate"},
    },
    {
        {.type=parameterCont, .number=lfoBPitch, .shortName="BAmp", .longName="LFO B Amplitude"},
        {.type=parameterCont, .number=lfoBRate, .shortName="BRte", .longName="LFO B Rate"},
        {.type=parameterCont, .number=spLfoBShape, .shortName="BShp", .longName="LFO B Shape"},
        {.type=parameterStep, .number=spLfoBspeed, .shortName="Bspd", .longName="LFO B Speed"},
        {.type=parameterStep, .number=spLfoBTrk, .shortName="BTrk", .longName="LFO B Tracking"},
        {.type=parameterStep, .number=spLfoBTrig, .shortName="BTrg", .longName="LFO B Trig"},
        {.type=parameterNone},
        {.type=parameterNone},
    },
};


static int uint16Compare(const void * a,const void * b)
{
	if (*(uint16_t*)a==*(uint16_t*)b)
		return 0;
	else if (*(uint16_t*)a < *(uint16_t*)b)
		return -1;
	else
		return 1;
}

static uint8_t debounce(uint8_t port, uint8_t button){

  uint8_t output = 0;


  if (!(GPIOC_PDIR&(1<<port))){
    if (interface.integrator[button] > 0)
      interface.integrator[button]--;
  }
  else if (interface.integrator[button] < DEBOUNCE_MAX)
      interface.integrator[button]++;

  if (interface.integrator[button] == 0)
      output = 0;
  else if (interface.integrator[button] >= DEBOUNCE_MAX){
    output = 1;
    interface.integrator[button] = DEBOUNCE_MAX;
  }

  return output;
}

static void handleUserInput(uint8_t input){

    const struct interfaceParam_s * parameter;
    parameter = &interfaceParameters[interface.page][input];
    uint8_t current_type;
    static char dv[4]={0};
    switch(parameter->type){
        case parameterNone:
            return;
        case parameterCont:
            cpParameterList[parameter->number]= interface.pot_value[input];
            current_type = 0;
            sprintf(dv,"%3d",cpParameterList[parameter->number]>>8);
            break;
        case parameterStep:
            uint8_t v = interface.pot_value[input]>>(16-spParameterBits[parameter->number]);
            spParameterList[parameter->number]= v;
            sprintf(dv,"%3d",spParameterList[parameter->number]);
            break;
    }
    if(input < NUM_OF_POTS/2) {
        cposition(input * 5 + 1, 1);
    }
    else {
        cposition((input - 4) * 5 + 1, 3);
    }
    putsLCD(dv);
    parameterChange();

  }

void interfaceUpdatePage(){
    cmd2LCD(0x01);
    delay(2);
    const char * space = " ";
    cposition(0,0);
    for (int i = 0; i < PARAMETER_PAGES/2; ++i) {
        //sprintf(dv, "%-4s", interfaceParameters[interface.page][i].shortName);
        if (interfaceParameters[interface.page][i].type!=parameterNone) {
            putsLCD(interfaceParameters[interface.page][i].shortName);
            putsLCD(space);
        }
    }

    cposition(0, 1);
    for (int i = 0; i < PARAMETER_PAGES/2; ++i) {
        static char dv[4]={0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.page][i].shortName);
        if (interfaceParameters[interface.page][i].type==parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[interface.page][i].number] >> 8);
            putsLCD(dv);
            putsLCD(space);
        } else if (interfaceParameters[interface.page][i].type == parameterStep){
            sprintf(dv, "%4d", spParameterList[interfaceParameters[interface.page][i].number]);
            putsLCD(dv);
            putsLCD(space);
        }
    }

    cposition(0,2);
    for (int i = PARAMETER_PAGES/2; i < PARAMETER_PAGES; ++i) {
        if (interfaceParameters[interface.page][i].type!=parameterNone) {
            putsLCD(interfaceParameters[interface.page][i].shortName);
            putsLCD(" ");
        }
    }

    cposition(0,3);
    for (int i = PARAMETER_PAGES/2; i < PARAMETER_PAGES; ++i) {
        static char dv[4]={0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.page][i].shortName);
        if (interfaceParameters[interface.page][i].type==parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[interface.page][i].number] >> 8);
            putsLCD(dv);
            putsLCD(space);
        } else if (interfaceParameters[interface.page][i].type == parameterStep){
            sprintf(dv, "%4d", spParameterList[interfaceParameters[interface.page][i].number]);
            putsLCD(dv);
            putsLCD(space);
        }
    }
}


static void readPotentiometers(){
    uint32_t new_value;
  	uint16_t tmp[NUM_OF_SAMPLES];
    uint8_t tmpAdress[] = {11, 10, 0, 1, 2, 3, 4, 5, 6, 7,};

    if (!analogBusy){
        GPIOD_PSOR = (interface.increament_potentiometer << 11);
        GPIOA_PCOR = (1 << 17);
        analogBusy = 1;
        ADC0_SC1A = 23;
    }

    if (ADC0_SC1A & ADC_SC1_COCO){
        interface.pot_samples[interface.increament_potentiometer]
        [interface.current_sample[interface.increament_potentiometer]]= ADC0_RA;
        GPIOA_PSOR = (1 << 17);
        GPIOD_PCOR = (interface.increament_potentiometer << 11);
        analogBusy = 0;

        memcpy(&tmp[0], &interface.pot_samples[interface.increament_potentiometer][0],
               NUM_OF_SAMPLES*sizeof(uint16_t));
        qsort(tmp, NUM_OF_SAMPLES, sizeof(uint16_t), uint16Compare);

        new_value = tmp[NUM_OF_SAMPLES/2];

        if (abs(new_value-interface.pot_value[interface.increament_potentiometer]) >= POT_THRESHOLD){
            if(tik - interface.pot_time[interface.increament_potentiometer] >= POT_TIME) {
                interface.pot_value[interface.increament_potentiometer] = new_value;
                handleUserInput(interface.increament_potentiometer);
            }
            else if (abs(new_value-interface.pot_value[interface.increament_potentiometer]) >= 2*POT_THRESHOLD){
                interface.pot_value[interface.increament_potentiometer] = new_value;
                handleUserInput(interface.increament_potentiometer);
            }
            interface.pot_time[interface.increament_potentiometer] = tik;
        }

        interface.increament_potentiometer=
                (interface.increament_potentiometer+1)%NUM_OF_POTS;

        interface.current_sample[interface.increament_potentiometer] =
                (interface.current_sample[interface.increament_potentiometer] + 1) %
                NUM_OF_SAMPLES;
    }


}

static void readButton(){

    //uint16_t reading = REG_PIOC_PDSR>>22;

    uint8_t output = debounce(10, 0);
    //test_variable++;
    if(!interface.lastButtonPress[0] && output){
        interface.page++;
        interface.page%=PARAMETER_PAGES;
        //test_variable = interface.page;
        interface.lastButtonPress[0] = output;
        updatePage = 1;
        return; //decuase you shouldn't be pressing more then one button.
      }
    interface.lastButtonPress[0] = output;

    output = debounce(11, 1);

    if (!interface.lastButtonPress[1] && output){
        interface.page--;
        interface.page%=PARAMETER_PAGES;
        //test_variable = interface.page;
        interface.lastButtonPress[1] = output;
        updatePage = 1;
        return;
      }

      interface.lastButtonPress[1] = output;
      //test_variable = interface.page;
}



void interfaceInit() {
    uint8_t tmp[] = {11, 10, 0, 1, 2, 3, 4, 5, 6, 7,};
    //memcpy(&interface.potAdress[0], &tmp[0], sizeof(uint8_t));
    GPIOA_PSOR = (1 << 17);

    for(unsigned long long &value: interface.pot_time){
        value=0;
    }

    for(unsigned short &value: interface.pot_value){
        value=0;
    }

    for(unsigned short &value: interface.current_sample){
        value=0;
    }

    openLCD();
    delay(200);
    const char *msg1="mphaines88/Roykeru";
    putsLCD(msg1);
    cposition(0,2);
    delay(2);
    const char *msg2="SX8 Synthesizer";
    putsLCD(msg2);
    delay(2000);
    //cmd2LCD(0x01);
    analogReadRes(16);

    interface.temp_parameter = 255;
    interface.page = 0;
    interfaceUpdatePage();
    interfaceUpdate();

}

void interfaceUpdate(){
    if (updatePage & !analogBusy){
        interfaceUpdatePage();
        updatePage = 0;
    }
    readPotentiometers();
    readButton();

}
