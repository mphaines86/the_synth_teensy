#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>


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

} continousParameters_t;

typedef enum {
  spOscAWave=0, spOscBWave=1, spOscSync=2,
   spFltrEnvTrig=3, spFltrEnvSpd=4,
   spAmpEnvTrig=5, spAmpEnvSpd=6,
   spAuxEnvTrig=7, spAuxEnvSpd=8,
   spLfoAShape=9, spLfoAspeed=10, spLfoATrk=11, spLfoATrig=12,
   spLfoBShape=13, spLfoBspeed=14, spLfoBTrk=15, spLfoBTrig=16,

  spCount

} steppedParameter_t;

extern uint16_t cpParameterList[cpCount];
extern uint16_t spParameterList[spCount];
extern uint8_t spParameterBits[spCount];
void interfaceInit();
void interfaceUpdate();
void interfaceUpdatePage();

#endif
