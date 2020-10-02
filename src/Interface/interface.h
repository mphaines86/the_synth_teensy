#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

#define NUM_OF_POTS 8
#define PARAMETER_PAGES 9

typedef  enum {
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
