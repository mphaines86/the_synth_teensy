#ifndef PARAMETER_H
#define PARAMETER_H

#include <stdint.h>

#define PARAMETER_PAGES 9

typedef enum {
    parameterNone=0, parameterCont, parameterStep, parameterCust
} interfaceParamType_e;

typedef enum {
    oscAfreq=0, oscACrse=1, oscAVol=2, oscAMod=3, oscARamp=4,
    oscBfreq=5, oscBCrse=6, oscBVol=7, oscBRamp=8, oscBMod=9, oscFMMod=10,
    oscAStart=11, oscAEnd=12, oscBStart=13, oscBEnd=14,
    oscAWvLfo=15, oscAWvRmp=16, oscBWvLfo=17, oscBWvRmp=18,
    fltrCutoff=19, fltrResonance=20, fltrEnvMnt=21, fltrMod=22,
    fltrAtt=23, fltrDec=24, fltrSus=25, fltrRel=26, fltrAmp=27,
    AmpAtt=28, AmpDec=29, AmpSus=30, AmpRel=31, AmpAmp=32,
    AuxAtt=33, AuxDec=34, AuxSus=35, AuxRel=36, AuxAmp=37,
    lfoAPitch=38, lfoARate=39,
    lfoBPitch=40, lfoBRate=41,
    rampRate=42, rampAmount=43,
  
    cpCount
  
  } ptContinuousParameters_t;
  
  typedef enum {
    spOscAWave=0, spOscBWave=1, spOscSync=2, spOscATyp=3, spOscBTyp=4,
     spFltrEnvTrig=5, spFltrEnvSpd=6,
     spAmpEnvTrig=7, spAmpEnvSpd=8,
     spAuxEnvTrig=9, spAuxEnvSpd=10,
     spLfoAShape=11, spLfoAspeed=12, spLfoATrk=13, spLfoATrig=14,
     spLfoBShape=15, spLfoBspeed=16, spLfoBTrk=17, spLfoBTrig=18,
  
    spCount
  
  } ptSteppedParameter_t;

  typedef enum {
    waveLength=0, waveLoop=1, wavePitchC5=2,

    cpWaveCount
} spContinuousParameters_t;

typedef enum {
    waveName=0
} spSteppedParameters;

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
                {.type=parameterCont, .number=oscAMod, .shortName="AMod", .longName="Osc A Lfo Mod", .values={""}},
                {.type=parameterCont, .number=oscARamp, .shortName="ARmp", .longName="Osc A Ramp Mod", .values={""}},
                {.type=parameterStep, .number=spOscSync, .shortName="Sync", .longName="Osc Sync", .values={"Off ", "Hard", "Ring", "Soft"}},
                {.type=parameterStep, .number=spOscAWave, .shortName="Awav", .longName="Osc A Waveform", .values={""}},
                {.type=parameterStep, .number=spOscATyp, .shortName="Type", .longName="Osc A Type", .values={"Smpl", "Wave"}},
        },
        {
                {.type=parameterCont, .number=oscBfreq, .shortName="BFrq", .longName="Osc B Frequancy", .values={""}},
                {.type=parameterCont, .number=oscBCrse, .shortName="BCrs", .longName="Osc B Coarse", .values={""}},
                {.type=parameterCont, .number=oscBVol, .shortName="BVol", .longName="Osc B Volume", .values={""}},
                {.type=parameterCont, .number=oscBMod, .shortName="BMod", .longName="Osc B Lfo Mod", .values={""}},
                {.type=parameterCont, .number=oscBRamp, .shortName="BRmp", .longName="Osc B Ramp Mod", .values={""}},
                {.type=parameterCont, .number=oscFMMod, .shortName="Fmod", .longName="Osc B FM Mod", .values={""}},
                {.type=parameterStep, .number=spOscBWave,.shortName="Bwav", .longName="Osc B Waveform", .values={""}},
                {.type=parameterStep, .number=spOscBTyp, .shortName="Type", .longName="Osc B Type", .values={"Smpl", "Wave"}},
        },
        {
                {.type=parameterCont, .number=oscAStart, .shortName="AStt", .longName="Osc A Tbl Strt", .values={""}},
                {.type=parameterCont, .number=oscAWvLfo, .shortName="AWMd", .longName="Osc A Wave Mod", .values={""}},
                {.type=parameterCont, .number=oscAWvRmp, .shortName="AWRp", .longName="Osc A Wave Rmp", .values={""}},
                {.type=parameterNone,},
                {.type=parameterCont, .number=oscBStart, .shortName="BStt", .longName="Osc B Tbl Strt", .values={""}},
                {.type=parameterCont, .number=oscBWvLfo, .shortName="BWMd", .longName="Osc B Wave Mod", .values={""}},
                {.type=parameterCont, .number=oscBWvRmp, .shortName="BWRp", .longName="Osc B Wave Rmp", .values={""}},
                {.type=parameterNone,},
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
                {.type=parameterCont, .number=rampRate, .shortName="RRte", .longName="Ramp Rate", .values={""}},
                {.type=parameterNone, },
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

extern uint16_t cpParameterList[cpCount];
extern uint16_t spParameterList[spCount];
extern uint8_t spParameterBits[spCount];
extern uint32_t cpSampleList[cpWaveCount];

#endif