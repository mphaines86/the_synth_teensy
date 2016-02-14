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
	uint8_t stageIncreament;
	uint8_t phase;
	uint8_t attackIncreament, decayIncreament, releaseIncreament;
	
	uint8_t speedShift;
	
	EnvelopeStage_t stage;
	};

void envelope_trigger(struct envelope_struct * envelope);
inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage);

EnvelopeStage_t env_getStage(struct envelope_struct * envelope);
void envelope_update(struct envelope_struct * envelope);


#endif /* INCFILE1_H_ */
