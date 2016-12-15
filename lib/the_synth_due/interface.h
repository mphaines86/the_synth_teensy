#ifndef INTERFACE_H
#define INTERFACE_H

#include "synth.h"

#define NUM_OF_POTS 10
#define NUM_OF_SAMPLES 10
#define POT_THRESHOLD 12

typedef enum {
  oscAfreq=1, oscAVol=2, oscAMod=3, oscSync=4,
  oscBfreq=5, oscBVol=6, oscBMod=7, oscDetune=8,
  fltrCutoff=9, fltrResonance=10, fltrEnvMnt=11, fltrMod=12,
  fltrAtt=13, fltrDec=14, fltrSus=15, fltrRel=16,
  AmpAtt=17, AmpDec=18, AmpSus=19, AmpRel=20,
  AuxAtt=21, AuxDec=22, AuxSus=23, AuxRel=24,
  lfoAPitch=25, lfoARate=26, lfoAAmount=27, lfoAShape=28,
  lfoBPitch=29, lfoBRate=30, lfoBAmount=31, lfoBShape=32,
  rampRate=33, rampAmount=34,

  cpCount

} continousParameters_t;

typedef enum {
  spOscAWave=1, spOscBWave=2, spOscSync=3,
  spLfoAShape=4, spLfoBShape=5, spLfoAspeed=6, spLfoBspeed=7,

  spCount

} steppedParameter_t;

uint16_t cpParameterList[cpCount];
uint16_t spParameterList[spCount];
void interfaceInit();
void interfaceUpdate();

#endif
