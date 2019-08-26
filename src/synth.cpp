/*
 * CFile1.c
 *
 * Created: 2/12/2016 10:27:58 PM
 *  Author: Michael Haines
 */
#include <system/HD44780.h>
#include "Arduino.h"
#include "tables_due.h"
#include "synth.h"
#include "Envelope.h"
#include "Osc.h"
#include "lfo.h"
#include "interface.h"
#include "ramp.h"
#include "util/delay.h"
#include "system/utilities.h"
#include "midi_l.h"


static struct{

	struct envelope_struct amplitudeEnvs[SYNTH_VOICE_COUNT];
	struct oscillator_struct oscillators[SYNTH_VOICE_COUNT];
	struct envelope_struct filterEnvs[SYNTH_VOICE_COUNT];
	struct envelope_struct resonantEnvs[SYNTH_VOICE_COUNT];
	struct lfo_struct pitchlfo[SYNTH_VOICE_COUNT];
	struct lfo_struct filterlfo[SYNTH_VOICE_COUNT];
	struct ramp_struct pitchramp[SYNTH_VOICE_COUNT];

	} synthesizer;


volatile uint16_t amplitude[SYNTH_VOICE_COUNT];           //-Wave amplitudes [0-255]
volatile uint16_t current_note[SYNTH_VOICE_COUNT];

volatile unsigned char divider = 0; //-Sample rate decimator for envelope
volatile uint64_t tik = 0;

volatile int16_t Pitch_bend[SYNTH_VOICE_COUNT];

volatile int noteTrigger[SYNTH_VOICE_COUNT];
volatile int noteDeath[SYNTH_VOICE_COUNT];
volatile int current_stage = 0;

volatile int32_t test_variable = 0;
volatile uint32_t timer = 0;

//******************************************************************************
//These are temporary variables and bound to change.
//******************************************************************************

uint16_t amp_out;
uint8_t  multiplexer = 0;
uint8_t aftertouch = 0;
int8_t global_detune = 0;
uint8_t cv7 = 0;
uint8_t cv70 = 0;
uint8_t cv2 = 0;
uint8_t cv12 = 0;
uint8_t cv = 0;
uint8_t ramp1 = 0;
volatile uint16_t filter_total = 0;
uint32_t time_stamps[] = {0, 0, 0, 0, 0, 0, 0, 0};

 byte notes[SYNTH_VOICE_COUNT];
 int free_notes[] = {0, 1, 2, 3, 4, 5, 6, 7};


//******************************************************************************
//  Audio driver interrupt
//******************************************************************************
void ftm1_isr(void){

    //timer = FTM1_CNT;
	//-------------------------------
	// Time division
	//-------------------------------
	divider++;
    if(!(divider%=SYNTH_VOICE_COUNT))
		tik++;


	//-------------------------------
	// Volume and Filter envelope generator
	//-------------------------------
	envelope_update(&synthesizer.amplitudeEnvs[divider]);
	envelope_update(&synthesizer.filterEnvs[divider]);
	envelope_update(&synthesizer.resonantEnvs[divider]);
	ramp_update(&synthesizer.pitchramp[divider]);
	//osc_setAmplitude(&synthesizer.oscillators[divider], amplitude[divider]);

	lfo_update(&synthesizer.filterlfo[divider]);
	filter_total = (env_getOutput(&synthesizer.filterEnvs[divider]) + (cpParameterList[fltrCutoff] >> 8) +
	aftertouch); //+ lfo_getOutput(&synthesizer.filterlfo[0]));
	if (filter_total > 255) filter_total = 255;
	//filter_total = 255;
	//test_variable = filter_total;
	//test_variable = ramp_getOutput(&synthesizer.pitchramp[0]);

	amplitude[divider] = env_getOutput(&synthesizer.amplitudeEnvs[divider]);
    osc_setAmplitude(&synthesizer.oscillators[divider], amplitude[divider]);
    //lfo_setLevel(&synthesizer.pitchlfo[divider], amplitude[divider]);
    // test_variable = amplitude[0];


    //
    GPIOB_PCOR = (1 << 23);
	//Delay_ns(2);
    GPIOB_PSOR = divider << 20;

	GPIOD_PCOR = (0b11 << 8);
	GPIOD_PCOR = (0x000000FF);
	GPIOD_PSOR = (filter_total); // filter_total;
	if (divider == 0) {
        GPIOE_PSOR = (1 << 26);
    }
	Delay_ns(1);
	GPIOD_PSOR = (1 << 9);
	Delay_ns(1);
	GPIOD_PCOR = (1 << 9);
	GPIOD_PSOR = (1 << 8);
	GPIOD_PCOR = (0x000000FF);
	GPIOD_PSOR = env_getOutput(&synthesizer.resonantEnvs[divider]);
	Delay_ns(1);
	GPIOD_PSOR = (1 << 9);
	delayMicroseconds(5);
	//Delay_ns(100);
	if (divider == 0) {
		GPIOE_PCOR = (1 << 26);
	}
	GPIOB_PSOR = (1 << 23);
	GPIOB_PCOR = (divider << 20);

	lfo_update(&synthesizer.pitchlfo[divider]);
    //lfo_setRate(&synthesizer.pitchlfo[divider], synthesizer.oscillators[divider].cv_pitch[0] - 1000);
	osc_setPitch(&synthesizer.oscillators[divider], synthesizer.oscillators[divider].cv_pitch[0] +
            (cpParameterList[oscAfreq] >> 8) + ((lfo_getOutput(&synthesizer.pitchlfo[divider]) * cpParameterList[oscAMod])>>12) +
            + ramp_getOutput(&synthesizer.pitchramp[divider]) + ((synthesizer.oscillators[divider].output[1] * cpParameterList[oscFMMod]) >> 8), 0);

	osc_setPitch(&synthesizer.oscillators[divider], synthesizer.oscillators[divider].cv_pitch[1] +
			(cpParameterList[oscBfreq] >>8) + ((lfo_getOutput(&synthesizer.pitchlfo[divider]) * cpParameterList[oscBMod])>>12) +
            + ramp_getOutput(&synthesizer.pitchramp[divider]), 1);

	//osc_setPitch(&synthesizer.oscillators[divider], synthesizer.oscillators[divider].cv_pitch[0],0);
	//osc_setPitch(&synthesizer.oscillators[divider], synthesizer.oscillators[divider].cv_pitch[1],1);

	//-------------------------------
	//  Synthesizer/audio mixer
	//-------------------------------

	int16_t output_sum = 0;
	for (auto &oscillator : synthesizer.oscillators) {
			osc_update(&oscillator);
			output_sum += osc_getOutput(&oscillator);
			//output_sum += lfo_getOutput(&synthesizer.pitchlfo[i]);
	}
    //test_variable = output_sum;
	output_sum = 127 + ((output_sum) >> 4);

	//test_variable = output_sum;

	GPIOC_PCOR = (0b11 << 8);
	GPIOC_PCOR = (0x000000FF);
	GPIOC_PSOR = output_sum;
	Delay_ns(1);
	GPIOC_PSOR = (1 << 9);
	Delay_ns(1);
	GPIOC_PCOR = (1 << 9);
	GPIOC_PSOR = (1 << 8);
	GPIOC_PCOR = (0x000000FF);
	GPIOC_PSOR = (cpParameterList[fltrResonance] >> 8);
	Delay_ns(1);
	GPIOC_PSOR = (1 << 9);

	osc_updateFrequancyTuningWord(&synthesizer.oscillators[divider]);
    //test_variable = FTM1_CNT - timer;

	FTM1_SC &= ~FTM_SC_TOF;
}
//------------------------------------------------------------------------------
//Update the interface and check for any human interactions.
//------------------------------------------------------------------------------

void interfaceCheck(){
	midi_read();
    //test_variable = synthesizer.oscillators[1].cv_pitch[1];
    //if(!(tik%2056)) {
        //test_variable++;
        //Serial.println(test_variable);
        //static char dv[10] = {0};
        //sprintf(dv,"%8d",test_variable);
        //cposition(0, 2);
        //putsLCD(dv);
    //}
	interfaceUpdate();
}

//------------------------------------------------------------------------------
// Anything below this line is temporary and subject to change with the
// exception of function synth_begin()
//------------------------------------------------------------------------------

void set_envelopes(){

	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
		envelope_setup(&synthesizer.amplitudeEnvs[i], 65535, 65535, 65535, 500, 0, 0, UINT16_MAX);
		envelope_setup(&synthesizer.filterEnvs[i], 65535, 65535, 65535, 1, 0, 0, UINT16_MAX);
		//envelope_setup(&synthesizer.resonantEnvs[i], 46,56,45333,19);
		envelope_setup(&synthesizer.resonantEnvs[i], 65535, 75, 65535, 500, 0, 0, UINT16_MAX);
		ramp_setup(&synthesizer.pitchramp[i], 0);
	}

	cpParameterList[fltrCutoff] = 65535;
	cpParameterList[fltrEnvMnt] = 0;
	cpParameterList[fltrAtt] = 0;
	cpParameterList[fltrDec] = 0;
	cpParameterList[fltrRel] = 0;
	cpParameterList[fltrSus] = 0;
	cpParameterList[fltrMod] = 0;
	cpParameterList[fltrResonance] = 0;
	cpParameterList[AuxAtt] = 0;
	cpParameterList[AuxDec] = 0;
	cpParameterList[AuxRel] = 0;
	cpParameterList[AuxSus] = 0;
	cpParameterList[AmpAtt] = 65535;
	cpParameterList[AmpDec] = 65535;
	cpParameterList[AmpRel] = 500;
	cpParameterList[AmpSus] = 65535;
	cpParameterList[rampAmount] = 0;
	cpParameterList[rampRate] = 0;
	cpParameterList[AmpAmp] = 255;
	cpParameterList[fltrAmp] = 255;
	cpParameterList[AuxAmp] = 255;
	spParameterList[spFltrEnvSpd] = 0;
	spParameterList[spFltrEnvTrig] = 0;
	spParameterList[spAmpEnvSpd] = 0;
	spParameterList[spAmpEnvTrig] = 0;
	spParameterList[spAuxEnvSpd] = 0;
	spParameterList[spAuxEnvTrig] = 0;

}


void set_oscillators(){
	int i = 0;
	cpParameterList[oscAVol] = 65535;
	cpParameterList[oscBVol] = 65535;
	cpParameterList[oscAfreq] = 0;
	cpParameterList[oscBfreq] = 0;
	cpParameterList[oscACrse] = 0;
	cpParameterList[oscBCrse] = 0;
	cpParameterList[oscAMod] = 0;
	cpParameterList[oscBMod] = 0;
	spParameterList[spOscSync] = 0;
	cpParameterList[oscFMMod] = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
		osc_setParameters(&synthesizer.oscillators[i], static_cast<oscSyncMode_t>(spParameterList[spOscSync]),
				cpParameterList[oscAVol], cpParameterList[oscBVol]);
		//setVoices(&synthesizer.oscillators[i], &string_C6, 0, 127);
		osc_setWaves(&synthesizer.oscillators[i], &waveStruct[8],0, 127, 0);
		osc_setWaves(&synthesizer.oscillators[i], &waveStruct[8],0, 127, 1);
	}

	//setVoices(&synthesizer.oscillators[7], &snare,0,127);

}

void set_lfo(){
	spParameterList[spLfoATrk] = 0;
	spParameterList[spLfoBTrk] = 0;
	int i = 0;
	for (i = 0; i< SYNTH_VOICE_COUNT; i++){
        lfo_init(&synthesizer.pitchlfo[i], lfoSine, 5000, 254, 0);
        lfo_init(&synthesizer.filterlfo[i], lfoSine, 5000, 254, 0);
		synthesizer.pitchlfo[i].key_follow = spParameterList[spLfoATrk];
		synthesizer.filterlfo[i].key_follow = spParameterList[spLfoBTrk];

	}
	cpParameterList[lfoARate] = 0;
	cpParameterList[lfoAPitch] = 0;
	spParameterList[spLfoATrig] = 0;
	spParameterList[spLfoAspeed] = 0;
	//cpParameterList[lfoAShape] = 1;
	cpParameterList[lfoBPitch] = 0;
	cpParameterList[lfoBRate] = 0;
	spParameterList[spLfoBTrig] = 0;
	spParameterList[spLfoBspeed] = 0;
	//cpParameterList[lfoBShape] = 1;

}

void synth_begin()
{
    //initialize interface
    test_variable=0;
    interfaceInit();
	midi_begin();

    FTM1_MODE |= FTM_MODE_WPDIS;
    FTM1_MODE |= FTM_MODE_FTMEN;
    FTM1_MODE |= FTM_MODE_INIT;
    FTM1_SC = 0;
    FTM1_CNT = 0;
    FTM1_MOD = F_BUS / (1 * FS) - 1;
    // FTM1_C0V = (F_BUS / 3000) / 32 - 1;
    FTM1_SC = FTM_SC_CLKS(0b01) | FTM_SC_PS(0b0) | FTM_SC_TOF | FTM_SC_TOIE;
    NVIC_ENABLE_IRQ(IRQ_FTM1);

    set_oscillators();
    set_envelopes();
    set_lfo();

}


void note_trigger(byte given_pitch, byte velocity) {

	int8_t reset_flag = 0;
	int8_t voice=-1;
	uint8_t i = 0;
	uint32_t oldest_time_stamp=UINT32_MAX;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
		if(free_notes[i] > -1){
			//test_variable = env_getStage(&synthesizer.amplitudeEnvs[i]);
			if (env_getStage(&synthesizer.amplitudeEnvs[i]) == DEAD){
					voice = i;
					break;
			}
			if(time_stamps[i]<oldest_time_stamp){
					oldest_time_stamp=time_stamps[i];
					voice = i;
			}
			if(current_note[i] == given_pitch){
				voice = i;
				//reset_flag = 1;
			}
		}
	}
	test_variable=voice;
	if (voice==-1){
		return;
	}
	//test_variable = voice;
	//amplitude[voice] = 254; //temp
	notes[voice] = given_pitch;
	free_notes[voice] = -1;
	current_stage += 1;
	current_note[voice] = given_pitch;

	uint16_t note_a = given_pitch + (cpParameterList[oscACrse]>>12);
	uint16_t note_b = given_pitch + (cpParameterList[oscBCrse]>>12);
	uint16_t temp_pitch[2] = {note_a, note_b};
	uint8_t osc_amp[2] = {(uint8_t) (cpParameterList[oscAVol]>>8), (uint8_t) (cpParameterList[oscBVol]>>8)};
	osc_trigger(&synthesizer.oscillators[voice], temp_pitch, given_pitch, osc_amp);
	lfo_trigger(&synthesizer.pitchlfo[voice], given_pitch, -12);
	//lfo_trigger(&synthesizer.pitchlfo[voice]);
	lfo_trigger(&synthesizer.filterlfo[voice]);
	ramp_trigger(&synthesizer.pitchramp[voice], cpParameterList[rampAmount]);

    envelope_trigger(&synthesizer.amplitudeEnvs[voice], 65535);
    envelope_trigger(&synthesizer.resonantEnvs[0], velocity * 516);
    envelope_trigger(&synthesizer.filterEnvs[0], cpParameterList[fltrEnvMnt]);
	//if(current_stage <=1){
	//}
	noteTrigger[voice] = 1;
}


void NoteRelease(byte given_pitch) {
	int i = 0;
	for(i = 0; i < SYNTH_VOICE_COUNT; i++){
        if(notes[i] == given_pitch){
            //amplitude[i] = 0; //temp
            noteDeath[i] = 1;
            free_notes[i] = i;
            time_stamps[i] = tik;
			envelope_setGate(&synthesizer.amplitudeEnvs[i], 0);
			envelope_setGate(&synthesizer.filterEnvs[i], 0);
			envelope_setGate(&synthesizer.resonantEnvs[i], 0);
            //envelope_setStage(&synthesizer.amplitudeEnvs[i],RELEASE);
            //envelope_setStage(&synthesizer.filterEnvs[i],RELEASE);
            //envelope_setStage(&synthesizer.resonantEnvs[i],RELEASE);

        }

	}

	/*current_stage -= 1;
	if (current_stage <= 0){
		envelope_setStage(&synthesizer.filterEnvs[0],RELEASE);
		envelope_setStage(&synthesizer.resonantEnvs[0],RELEASE);
		current_stage = 0;
	}*/
}

void refreshEnvelopes(){

    for(uint8_t i=0; i<SYNTH_VOICE_COUNT;++i){
		envelope_setup(&synthesizer.amplitudeEnvs[i], cpParameterList[AmpAtt], cpParameterList[AmpDec],
					   cpParameterList[AmpSus], cpParameterList[AmpRel], spParameterList[spAmpEnvSpd],
					   spParameterList[spAmpEnvTrig], cpParameterList[AmpAmp]);
		envelope_setup(&synthesizer.filterEnvs[i], cpParameterList[fltrAtt], cpParameterList[fltrDec],
					   cpParameterList[fltrSus], cpParameterList[fltrRel], spParameterList[spFltrEnvSpd],
					   spParameterList[spFltrEnvTrig], cpParameterList[fltrAmp]);
		envelope_setup(&synthesizer.resonantEnvs[i], cpParameterList[AuxAtt], cpParameterList[AuxDec],
					   cpParameterList[AuxSus], cpParameterList[AuxRel], spParameterList[spAuxEnvSpd],
					   spParameterList[spAuxEnvTrig], cpParameterList[AuxAmp]);
		ramp_setup(&synthesizer.pitchramp[i], cpParameterList[rampRate]);
    }
}

void refreshOscillators(){

    for (uint8_t i=0; i < SYNTH_VOICE_COUNT; i++) {
		osc_setSync(&synthesizer.oscillators[i], static_cast<oscSyncMode_t>(spParameterList[spOscSync]));
		osc_setWaves(&synthesizer.oscillators[i], &waveStruct[spParameterList[spOscAWave]], 0, 127, 0);
		osc_setWaves(&synthesizer.oscillators[i], &waveStruct[spParameterList[spOscBWave]], 0, 127, 1);
	}
}

void refreshLfos(){
	for (uint8_t i = 0; i < SYNTH_VOICE_COUNT; ++i) {

		synthesizer.pitchlfo[i].key_follow = spParameterList[spLfoATrk];
		synthesizer.filterlfo[i].key_follow = spParameterList[spLfoBTrk];
        lfo_init(&synthesizer.pitchlfo[i], static_cast<lfoShape_t>(spParameterList[spLfoAShape]),
                 cpParameterList[lfoAPitch], cpParameterList[lfoARate], spParameterList[spLfoAspeed]);
        lfo_init(&synthesizer.filterlfo[i], static_cast<lfoShape_t>(spParameterList[spLfoBShape]),
                 cpParameterList[lfoBPitch], cpParameterList[lfoBRate], spParameterList[spLfoBspeed]);
	}
}

void parameterChange() {
	refreshOscillators();
	refreshLfos();
	refreshEnvelopes();
}
