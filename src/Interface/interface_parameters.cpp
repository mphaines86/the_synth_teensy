//
// Created by michael on 11/9/19.
//

#include <stdint.h>
#include <system/HD44780.h>
#include "interface_parameters.h"
#include "interface.h"

struct interfaceParam_s{
    interfaceParamType_e type;
    uint8_t number;
    const char * shortName;
    const char * longName;
    const char * values[8];
};

const struct interfaceParam_s interfaceParameters[PARAMETER_PAGES][8] = {
        {
                {.type=parameterCont, .number=oscAfreq, .shortName="AFrq", .longName="Osc A Frequancy", .values={""}},
                {.type=parameterCont, .number=oscACrse, .shortName="ACrs", .longName="Osc A Coarse", .values={""}},
                {.type=parameterCont, .number=oscAVol, .shortName="AVol", .longName="Osc A Volume", .values={""}},
                {.type=parameterCont, .number=oscAMod, .shortName="AMod", .longName="Osc A Mod", .values={""}},
                {.type=parameterStep, .number=spOscSync, .shortName="Sync", .longName="Osc Sync", .values={"Off ", "On  "}},
                {.type=parameterStep, .number=spOscAWave, .shortName="Awav", .longName="Osc A Waveform", .values={""}},
                {.type=parameterNone},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=oscBfreq, .shortName="BFrq", .longName="Osc B Frequancy", .values={""}},
                {.type=parameterCont, .number=oscBCrse, .shortName="BCrs", .longName="Osc B Coarse", .values={""}},
                {.type=parameterCont, .number=oscBVol, .shortName="BVol", .longName="Osc B Volume", .values={""}},
                {.type=parameterCont, .number=oscBMod, .shortName="BMod", .longName="Osc B Mod", .values={""}},
                {.type=parameterCont, .number=oscFMMod, .shortName="Fmod", .longName="Osc B FM Mod", .values={""}},
                {.type=parameterStep, .number=spOscBWave,.shortName="Bwav", .longName="Osc B Waveform", .values={""}},
                {.type=parameterStep, .number=spOscRing, .shortName="Rmod", .longName="Ring Mod", .values={"Off ", "On  "}},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=fltrCutoff, .shortName="FCut", .longName="Filter Cutoff", .values={""}},
                {.type=parameterCont, .number=fltrResonance, .shortName="FRes", .longName="Filter Resonance", .values={""}},
                {.type=parameterCont, .number=fltrEnvMnt, .shortName="FEnv", .longName="Filter Env Amount", .values={""}},
                {.type=parameterCont, .number=fltrMod, .shortName="FMod", .longName="Filter Modulation", .values={""}},
                {.type=parameterNone},
                {.type=parameterNone},
                {.type=parameterNone},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=fltrAtt, .shortName="FAtk", .longName="Filter Attack", .values={""}},
                {.type=parameterCont, .number=fltrDec, .shortName="FDec", .longName="Filter Decay", .values={""}},
                {.type=parameterCont, .number=fltrSus, .shortName="FSus", .longName="Filter Sustain", .values={""}},
                {.type=parameterCont, .number=fltrRel, .shortName="FRel", .longName="Filter Release", .values={""}},
                {.type=parameterCont, .number=fltrAmp, .shortName="FAmp", .longName="Filter Env Amp", .values={""}},
                {.type=parameterStep, .number=spFltrEnvTrig, .shortName="FTrg", .longName="Filter Env Trig", .values={"Sngl", "Mlti"}},
                {.type=parameterStep, .number=spFltrEnvSpd, .shortName="FSpd", .longName="Filter Env Speed", .values={"Fast", "Slow"}},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=AmpAtt, .shortName="AAtk", .longName="Ampitude Attack", .values={""}},
                {.type=parameterCont, .number=AmpDec, .shortName="ADec", .longName="Amplitude Decay", .values={""}},
                {.type=parameterCont, .number=AmpSus, .shortName="ASus", .longName="Amplitude Sustain", .values={""}},
                {.type=parameterCont, .number=AmpRel, .shortName="ARel", .longName="Amplitude Release", .values={""}},
                {.type=parameterCont, .number=AmpAmp, .shortName="AAmp", .longName="Amplitude Env Amp", .values={""}},
                {.type=parameterStep, .number=spAmpEnvTrig, .shortName="ATrg", .longName="Amplitude Env Trig", .values={"Sngl","Mlti"}},
                {.type=parameterStep, .number=spAmpEnvSpd, .shortName="ASpd", .longName="Amplitude Env Speed", .values={"Fast", "Slow"}},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=AuxAtt, .shortName="XAtk", .longName="Auxiliry Attack", .values={""}},
                {.type=parameterCont, .number=AuxDec, .shortName="XDec", .longName="Auxiliry Decay", .values={""}},
                {.type=parameterCont, .number=AuxSus, .shortName="XSus", .longName="Auxiliry Sustain", .values={""}},
                {.type=parameterCont, .number=AuxRel, .shortName="XRel", .longName="Auxiliry Release", .values={""}},
                {.type=parameterCont, .number=AuxAmp, .shortName="XAmp", .longName="Auxiliry Env Amp", .values={""}},
                {.type=parameterStep, .number=spAuxEnvTrig, .shortName="XTrg", .longName="Auxiliry Env Trig", .values={"Sngl", "Mlti"}},
                {.type=parameterStep, .number=spAuxEnvSpd, .shortName="XSpd", .longName="Auxiliry Env Speed", .values={"Fast", "Slow"}},
                {.type=parameterNone},
        },
        {
                {.type=parameterCont, .number=lfoAPitch, .shortName="AAmp", .longName="LFO A Amplitude", .values={""}},
                {.type=parameterCont, .number=lfoARate, .shortName="ARte", .longName="LFO A Rate", .values={""}},
                {.type=parameterStep, .number=spLfoAShape, .shortName="AShp", .longName="LFO A Shape", .values={"Sqr ","Tri ", "Saw ", "Sine"}},
                {.type=parameterStep, .number=spLfoAspeed, .shortName="Aspd", .longName="LFO A Speed", .values={"Fast", "Slow"}},
                {.type=parameterStep, .number=spLfoATrk, .shortName="ATrk", .longName="LFO A Tracking", .values={"Off ", "On  "}},
                {.type=parameterStep, .number=spLfoATrig, .shortName="ATrg", .longName="LFO A Trig", .values={"Sngl", "Mlti"}},
                {.type=parameterCont, .number=rampAmount, .shortName="RAmt", .longName="Ramp Amplitude", .values={""}},
                {.type=parameterCont, .number=rampRate, .shortName="RRte", .longName="Ramp Rate", .values={""}},
        },
        {
                {.type=parameterCont, .number=lfoBPitch, .shortName="BAmp", .longName="LFO B Amplitude", .values={""}},
                {.type=parameterCont, .number=lfoBRate, .shortName="BRte", .longName="LFO B Rate", .values={""}},
                {.type=parameterStep, .number=spLfoBShape, .shortName="BShp", .longName="LFO B Shape", .values={"Sqr ", "Tri ", "Saw ", "Sine" }},
                {.type=parameterStep, .number=spLfoBspeed, .shortName="Bspd", .longName="LFO B Speed", .values={"Fast", "Slow"}},
                {.type=parameterStep, .number=spLfoBTrk, .shortName="BTrk", .longName="LFO B Tracking", .values={"Off ", "On  "}},
                {.type=parameterStep, .number=spLfoBTrig, .shortName="BTrg", .longName="LFO B Trig", .values={"Sngl", "Mlti"}},
                {.type=parameterNone},
                {.type=parameterNone},
        },
};

void interfaceParameterHandleUserInput(int8_t input, uint16_t pot_value, uint8_t paramPage) {

    uint8_t parameterNumber = interfaceParameters[paramPage][input].number;

    static char dv[5] = {0};
    switch (interfaceParameters[paramPage][input].type) {
        case parameterNone:
            return;
        case parameterCont:
            cpParameterList[parameterNumber] = pot_value;
            sprintf(dv, "%4d", cpParameterList[parameterNumber] >> 8);
            break;
        case parameterStep: {
            uint8_t v = pot_value >> (16 - spParameterBits[parameterNumber]);
            spParameterList[parameterNumber] = v;
            if (!(interfaceParameters[paramPage][input].number == spOscAWave ||
                  interfaceParameters[paramPage][input].number == spOscBWave)){

                sprintf(dv, "%4s", interfaceParameters[paramPage][input].values[spParameterList[parameterNumber]]);

            }else{
                sprintf(dv, "%4d", spParameterList[parameterNumber]);
            }
            break;
        }
        default:
            break;
    }
    if (input < NUM_OF_POTS / 2) {
        cposition(input * 5, 1);
    } else {
        cposition((input - 4) * 5, 3);
    }
    putsLCD(dv);
}

void interfaceParameterUpdatePage(uint8_t paramPage) {

    cmd2LCD(0x01);
    delay(2);
    const char *space = " ";
    cposition(0, 0);
    for (int i = 0; i < PARAMETER_PAGES / 2; ++i) {
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type != parameterNone) {
            putsLCD(interfaceParameters[paramPage][i].shortName);
            putsLCD(space);
        }
    }

    cposition(0, 1);
    for (int i = 0; i < PARAMETER_PAGES / 2; ++i) {
        static char dv[5] = {0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type == parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[paramPage][i].number] >> 8);
            putsLCD(dv);
            putsLCD(space);
        } else if (interfaceParameters[paramPage][i].type == parameterStep) {
            //sprintf(dv, "%4d", interfaceParameters[interface.param_page][i].values[spParameterList[interfaceParameters[interface.param_page][i].number]]);
            putsLCD(interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
            putsLCD(space);
        }
    }

    cposition(0, 2);
    for (int i = PARAMETER_PAGES / 2; i < PARAMETER_PAGES; ++i) {
        if (interfaceParameters[paramPage][i].type != parameterNone) {
            putsLCD(interfaceParameters[paramPage][i].shortName);
            putsLCD(space);
        }
    }

    cposition(0, 3);
    for (int i = PARAMETER_PAGES / 2; i < PARAMETER_PAGES; ++i) {
        static char dv[5] = {0};
        //sprintf(dv, "%-4s", interfaceParameters[interface.param_page][i].shortName);
        if (interfaceParameters[paramPage][i].type == parameterCont) {
            sprintf(dv, "%4d", cpParameterList[interfaceParameters[paramPage][i].number] >> 8);
            putsLCD(dv);
            putsLCD(space);
        } else if (interfaceParameters[paramPage][i].type == parameterStep) {
            //sprintf(dv, "%4d", spParameterList[interfaceParameters[interface.param_page][i].number]);
            if (!(interfaceParameters[paramPage][i].number == spOscAWave ||
                  interfaceParameters[paramPage][i].number == spOscBWave)) {
                //putsLCD(interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
                sprintf(dv, "%4s", interfaceParameters[paramPage][i].values[spParameterList[interfaceParameters[paramPage][i].number]]);
            }else{
                sprintf(dv, "%4d", spParameterList[interfaceParameters[paramPage][i].number]);
            }
            putsLCD(dv);
            putsLCD(space);
        }
    }
}

