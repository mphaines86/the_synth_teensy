/*
 * IncFile1.h
 *
 * Created: 2/12/2016 9:25:47 PM
 *  Author: Michael Haines
 */ 


#ifndef ENVELOPE_H_
#define ENVELOPE_H_

#include "synth.h"

typedef enum {
	ATTACK = 0,
	DECAY = 1,
	SUSTAIN = 2,
	RELEASE = 3,
	DEAD = 4,
	NUM_SEGMENTS,
} EnvelopeStage_t;

struct envelope_struct {
	uint16_t stageIncreament;
	uint16_t phase;
	uint16_t attackIncreament, decayIncreament, releaseIncreament;
	
	uint16_t sustainCV,levelCV;
	uint16_t attackCV,decayCV,releaseCV;
	uint16_t stageLevel,stageAdd,stageMul;
	uint16_t speedShift;
	uint16_t output;
	
	EnvelopeStage_t stage;
	};

void envelope_trigger(struct envelope_struct * envelope);
static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage);

void setValues (struct envelope_struct * envelope, uint16_t stageinc, uint16_t attack, uint16_t decay, uint16_t release, uint16_t level, uint16_t sustain);
uint16_t env_getOutput(struct envelope_struct * envelope);
EnvelopeStage_t env_getStage(struct envelope_struct * envelope);
void envelope_update(struct envelope_struct * envelope);


#endif /* INCFILE1_H_ */
