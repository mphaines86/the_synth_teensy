#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

#define NUM_OF_POTS 8
#define PARAMETER_PAGES 8

typedef  enum {
    parameterNone=0, parameterCont, parameterStep, parameterCust
} interfaceParamType_e;

typedef enum {
  oscAfreq=0, oscACrse=1, oscAVol=2, oscAMod=3,
  oscBfreq=4, oscBCrse=5, oscBVol=6, oscBMod=7, oscFMMod=8,
  fltrCutoff=9, fltrResonance=10, fltrEnvMnt=11, fltrMod=12,
  fltrAtt=13, fltrDec=14, fltrSus=15, fltrRel=16, fltrAmp=17,
  AmpAtt=18, AmpDec=19, AmpSus=20, AmpRel=21, AmpAmp=22,
  AuxAtt=23, AuxDec=24, AuxSus=25, AuxRel=26, AuxAmp=27,
  lfoAPitch=28, lfoARate=29,
  lfoBPitch=30, lfoBRate=31,
  rampRate=32, rampAmount=33,

  cpCount

} ptContinuousParameters_t;

typedef enum {
  spOscAWave=0, spOscBWave=1, spOscSync=2, spOscRing=3,
   spFltrEnvTrig=4, spFltrEnvSpd=5,
   spAmpEnvTrig=6, spAmpEnvSpd=7,
   spAuxEnvTrig=8, spAuxEnvSpd=9,
   spLfoAShape=10, spLfoAspeed=11, spLfoATrk=12, spLfoATrig=13,
   spLfoBShape=14, spLfoBspeed=15, spLfoBTrk=16, spLfoBTrig=17,

  spCount

} ptSteppedParameter_t;

typedef enum {
    waveLength=0, waveLoop=1, wavePitchC5=2,

    cpWaveCount
} spContinuousParameters_t;

typedef enum {
    waveName=0
} spSteppedParameters;

extern uint16_t cpParameterList[cpCount];
extern uint16_t spParameterList[spCount];
extern uint8_t spParameterBits[spCount];
extern uint32_t cpSampleList[cpWaveCount];
void interfaceInit();
void interfaceUpdate();
void interfaceUpdatePage();
uint16_t interfaceGetPotValues(uint8_t input);
void interfaceUpdateNoteList(int8_t note, uint8_t state);

#endif
