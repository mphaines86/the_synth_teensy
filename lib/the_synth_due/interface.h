#ifndef INTERFACE_H
#define INTERFACE_H

#include "synth.h"

#define NUM_OF_POTS 10
#define NUM_OF_SAMPLES 10
#define POT_THRESHOLD 12

typedef enum {
  oscAfreq=1, oscAVol=2, oscAMod=3,
  oscBfreq=4, oscBVol=5, oscBMod=6, oscDetune=7,
  fltrCutoff=8, fltrResonance=9, fltrEnvMnt=10, fltrMod=1,
  fltrAtt=12, fltrDec=13, fltrSus=14, fltrRel=15,
  AmpAtt=16, AmpDec=17, AmpSus=18, AmpRel=19,
  AuxAtt=20, AuxDec=21, AuxSus=22, AuxRel=23,
  lfoAPitch=24, lfoARate=25, lfoAAmount=26, lfoAShape=27,
  lfoBPitch=28, lfoBRate=29, lfoBAmount=30, lfoBShape=31,

  count

} parameters_t;

uint16_t parameterList[count];

void interfaceInit();
void interfaceUpdate();

#endif
