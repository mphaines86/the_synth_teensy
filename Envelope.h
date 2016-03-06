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
	ATTACK = 1,
	DECAY = 2,
	SUSTAIN = 3,
	RELEASE = 4,
	DEAD = 0,
	NUM_SEGMENTS,
} EnvelopeStage_t;

struct envelope_struct {
	uint32_t stageIncreament;
	uint32_t phase;
	uint32_t attackIncreament, decayIncreament, releaseIncreament;
	
	uint16_t sustainCV,levelCV;
	uint16_t attackCV,decayCV,releaseCV;
	uint16_t stageLevel,stageAdd,stageMul;
	uint16_t speedShift;
	uint16_t output;
	
	EnvelopeStage_t stage;
	};

void envelope_trigger(struct envelope_struct * envelope, uint16_t level);
static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage);
void envelope_setStage(struct envelope_struct * envelope, EnvelopeStage_t stage);
void envelope_setup(struct envelope_struct * envelope, uint32_t attack, uint32_t decay, uint16_t sustain, uint32_t release);

uint16_t env_getOutput(struct envelope_struct * envelope);
EnvelopeStage_t env_getStage(struct envelope_struct * envelope);
void envelope_update(struct envelope_struct * envelope);


#endif /* INCFILE1_H_ */
