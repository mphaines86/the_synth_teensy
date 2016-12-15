#include <Arduino.h>
#include "interface.h"
#include "system/HD44780.h"

#define TIME_BUFFER 100
#define DEBOUNCE_MAX 4

static struct {
    uint16_t pot_samples[NUM_OF_POTS][NUM_OF_SAMPLES];
    uint8_t increament_potentiometer;
    uint16_t pot_value[NUM_OF_POTS];
    uint16_t current_sample[NUM_OF_POTS];
    uint8_t potAdress[NUM_OF_POTS];

    uint8_t integrator[2];
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

const struct interfaceParam_s interfaceParameters[4][10] = {
    {
        {.type=parameterCont, .number=oscAfreq, .shortName="AFrq", .longName="Osc A Frequancy"},
        {.type=parameterCont, .number=oscAVol, .shortName="AVol", .longName="Osc A Volume"},
        {.type=parameterCont, .number=oscAMod, .shortName="Amod", .longName="Osc A Modulation"},
        {.type=parameterCont, .number=oscSync, .shortName="Sync", .longName="Osc Sync"},
        {.type=parameterStep, .number=spOscAWave, .shortName="Awav", .longName="Osc A Waveform"},
        {.type=parameterCont, .number=oscBfreq, .shortName="BFrq", .longName="Osc B Frequancy"},
        {.type=parameterCont, .number=oscBVol, .shortName="BVol", .longName="Osc B Volume"},
        {.type=parameterCont, .number=oscBMod, .shortName="Bmod", .longName="Osc B Modulation"},
        {.type=parameterCont, .number=oscDetune, .shortName="Dtne", .longName="Osc Detune"},
        {.type=parameterStep, .number=spOscBWave,.shortName="Bwav", .longName="Osc B Waveform"},
    },
    {
        {.type=parameterCont, .number=fltrCutoff, .shortName="FCut", .longName="Filter Cutoff"},
        {.type=parameterCont, .number=fltrResonance, .shortName="FRes", .longName="Filter Resonance"},
        {.type=parameterCont, .number=fltrEnvMnt, .shortName="FEnv", .longName="Filter Env Amount"},
        {.type=parameterCont, .number=fltrMod, .shortName="FMod", .longName="Filter Modulation"},
        {.type=parameterNone},
        {.type=parameterCont, .number=fltrAtt, .shortName="FAtk", .longName="Filter Attack"},
        {.type=parameterCont, .number=fltrDec, .shortName="FDec", .longName="Filter Decay"},
        {.type=parameterCont, .number=fltrSus, .shortName="FSus", .longName="Filter Sustain"},
        {.type=parameterCont, .number=fltrRel, .shortName="FRel", .longName="Filter Release"},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=AmpAtt, .shortName="ACut", .longName="Ampitude Attack"},
        {.type=parameterCont, .number=AmpDec, .shortName="ARes", .longName="Amplitude Decay"},
        {.type=parameterCont, .number=AmpSus, .shortName="AEnv", .longName="Amplitude Sustain"},
        {.type=parameterCont, .number=AmpRel, .shortName="AMod", .longName="Amplitude Release"},
        {.type=parameterNone},
        {.type=parameterCont, .number=AuxAtt, .shortName="XAtk", .longName="Auxiliry Attack"},
        {.type=parameterCont, .number=AuxDec, .shortName="XDec", .longName="Auxiliry Decay"},
        {.type=parameterCont, .number=AuxSus, .shortName="XSus", .longName="Auxiliry Sustain"},
        {.type=parameterCont, .number=AuxRel, .shortName="XRel", .longName="Auxiliry Release"},
        {.type=parameterNone},
    },
    {
        {.type=parameterCont, .number=lfoAPitch, .shortName="AAmp", .longName="LFO A Amplitude"},
        {.type=parameterCont, .number=lfoARate, .shortName="ARte", .longName="LFO A Rate"},
        {.type=parameterCont, .number=lfoAShape, .shortName="AShp", .longName="LFO A Shape"},
        {.type=parameterCont, .number=lfoBPitch, .shortName="BAmp", .longName="LFO B Amplitude"},
        {.type=parameterCont, .number=lfoBRate, .shortName="BRte", .longName="LFO B Rate"},
        {.type=parameterCont, .number=lfoBShape, .shortName="BShp", .longName="LFO B Shape"},
        {.type=parameterCont, .number=rampAmount, .shortName="RAmt", .longName="Ramp Amplitude"},
        {.type=parameterCont, .number=rampRate, .shortName="RRte", .longName="Ramp Rate"},
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

  if (!(REG_PIOC_PDSR&(1<<port))){
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
    static char dv[10]={0};
    switch(parameter->type){
      case parameterNone:
        return;
      case parameterCont:
        cpParameterList[parameter->number]= interface.pot_value[input];
        current_type = 0;
        sprintf(dv,"%4d",cpParameterList[parameter->number]);
        cposition(0, 3);
        putsLCD(dv);
        potChange(parameter->number, current_type);
        break;
      case parameterStep:
        spParameterList[parameter->number]= interface.pot_value[input];
        current_type = 1;
        sprintf(dv,"%4d",spParameterList[parameter->number]);
        cposition(0, 3);
        putsLCD(dv);
        potChange(parameter->number, current_type);
        break;
    }

    if(interface.temp_parameter != parameter->number){
      static char dx[20]={0};
      sprintf(dx,"%-20s", parameter->longName);
      cposition(0, 2);
      putsLCD(dx);
      interface.temp_parameter = parameter->number;
    }
  }


static void readPotentiometers(){
    uint32_t new_value;
  	uint16_t tmp[NUM_OF_SAMPLES];
    uint8_t tmpAdress[] = {11, 10, 0, 1, 2, 3, 4, 5, 6, 7,};

    interface.increament_potentiometer=
      (interface.increament_potentiometer+1)%NUM_OF_POTS;

    interface.current_sample[interface.increament_potentiometer] =
      (interface.current_sample[interface.increament_potentiometer] + 1) %
      NUM_OF_SAMPLES;

    interface.pot_samples[interface.increament_potentiometer][interface.current_sample[interface.increament_potentiometer]]=
    adc_get_channel_value(ADC, tmpAdress[interface.increament_potentiometer]);
    memcpy(&tmp[0], &interface.pot_samples[interface.increament_potentiometer][0],
      NUM_OF_SAMPLES*sizeof(uint16_t));
    qsort(tmp, NUM_OF_SAMPLES, sizeof(uint16_t), uint16Compare);

    new_value = tmp[NUM_OF_SAMPLES/2];

    if (abs(new_value-interface.pot_value[interface.increament_potentiometer]) >= POT_THRESHOLD){
        interface.pot_value[interface.increament_potentiometer] = new_value;
        handleUserInput(interface.increament_potentiometer);
    }
}

static void readButton(){

    //uint16_t reading = REG_PIOC_PDSR>>22;

    uint8_t output = debounce(22, 0);
    //test_variable++;
    if(!interface.lastButtonPress[0] && output){
        interface.page++;
        interface.page%=4;
        //test_variable = interface.page;
        interface.lastButtonPress[0] = output;
        return; //decuase you shouldn't be pressing more then one button.
      }
    interface.lastButtonPress[0] = output;

    output = debounce(23, 1);

    if (!interface.lastButtonPress[1] && output){
        interface.page--;
        interface.page%=4;
        //test_variable = interface.page;
        interface.lastButtonPress[1] = output;
        return;
      }

      interface.lastButtonPress[1] = output;
      //test_variable = interface.page;
}



void interfaceInit() {
    uint8_t tmp[] = {11, 10, 0, 1, 2, 3, 4, 5, 6, 7,};
    //memcpy(&interface.potAdress[0], &tmp[0], sizeof(uint8_t));

    openLCD();
    delay(200);
    const char *msg1="mphaines88/Roykeru";
    putsLCD(msg1);
    cposition(0,2);
    const char *msg2="SX8 Synthesizer";
    putsLCD(msg2);
    delay(2500);
    cmd2LCD(0x01);

    pmc_enable_periph_clk(ID_ADC); // To use peripheral, we must enable clock distributon to it
    adc_init(ADC, SystemCoreClock, ADC_FREQ_MIN, ADC_STARTUP_FAST); // initialize, set maximum posibble speed
    adc_disable_interrupt(ADC, 0xFFFFFFFF);
    adc_set_resolution(ADC, ADC_12_BITS);
    adc_configure_power_save(ADC, 0, 0); // Disable sleep
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1); // Set timings - standard values
    adc_set_bias_current(ADC, 1); // Bias current - maximum performance over current consumption
    adc_stop_sequencer(ADC); // not using it
    adc_disable_tag(ADC); // it has to do with sequencer, not using it
    adc_disable_ts(ADC); // deisable temperature sensor
    adc_configure_trigger(ADC, ADC_TRIG_SW, 1); // triggering from software, freerunning mode
    adc_disable_all_channel(ADC);
    int i = 0;
    for (i=0; i < 10; i++){
          adc_disable_channel_differential_input(ADC, tmp[i]);
          adc_enable_channel(ADC, tmp[i]); // just one channel enabled
    }
    adc_start(ADC);

    interfaceUpdate();
    interface.temp_parameter = 255;
    interface.page = 0;

}

void interfaceUpdate(){

  readPotentiometers();
  readButton();

}
