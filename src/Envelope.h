/*
 * Envelope.h
 *
 * Created: 2/12/2016 9:25:47 PM
 *  Author: Michael Haines
 */


#ifndef ENVELOPE_H
#define ENVELOPE_H

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
	uint32_t stageIncreament;
	uint32_t phase;
	uint32_t attackIncreament, decayIncreament, releaseIncreament;

	uint16_t sustainCV,levelCV;
	uint16_t stageAdd,stageMul;
	uint32_t stageLevel;
	uint8_t trigger;
	uint16_t amplitude;
	uint16_t output;

	EnvelopeStage_t stage;
	};

//extern inline void envelope_trigger(struct envelope_struct * envelope, uint32_t level);
static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage);
extern void envelope_setStage(struct envelope_struct * envelope, EnvelopeStage_t stage);
extern void envelope_setup(struct envelope_struct *envelope, uint32_t attack, uint32_t decay, uint16_t sustain,
						   uint32_t release,
						   uint8_t speed, uint8_t trigger, uint16_t amp);
extern void envelope_setGate(struct envelope_struct * envelope, uint8_t gate);
//extern inline uint16_t env_getOutput(struct envelope_struct * envelope);
//extern inline EnvelopeStage_t env_getStage(struct envelope_struct * envelope);
extern void envelope_update(struct envelope_struct * envelope);


inline uint16_t env_getOutput(struct envelope_struct * envelope)
{
	return envelope->output;
}

inline EnvelopeStage_t env_getStage(struct envelope_struct * envelope){
	return envelope->stage;
}


static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage){

	switch(stage){

		case ATTACK:
			envelope->stageAdd = (envelope->stageLevel * envelope->levelCV) >> 16;
			envelope->stageMul = ((65535 - envelope->stageLevel) * envelope->levelCV )>> 16;
			envelope->stageIncreament=envelope->attackIncreament;
			break;
		case DECAY:
			envelope->stageAdd = (envelope->sustainCV * envelope->levelCV) >> 16;
			envelope->stageMul = ((65535 - envelope->sustainCV) * envelope->levelCV )>> 16;
			envelope->stageIncreament=envelope->decayIncreament;
			break;
		case SUSTAIN:
			envelope->stageAdd = 0;
			envelope->stageMul = envelope->levelCV;
			envelope->stageIncreament = 0;
			break;
		case RELEASE:
			//envelope->levelCV = ((uint16_t)envelope->output<<8);
			envelope->stageAdd = 0;
			envelope->stageMul = (envelope->stageLevel * envelope->levelCV)>> 16;
			envelope->stageIncreament=envelope->releaseIncreament;
			break;
		default:
			envelope->stageAdd=0;
			envelope->stageMul=0;
			envelope->stageIncreament=0;
	}
}


inline void envelope_trigger(struct envelope_struct * env, uint16_t level){
	env->levelCV = level;
	if (env->trigger){
		envelope_setGate(env, 1);
	}
	else {
		env->stage = ATTACK;
		env->phase = 0;
		env->stageLevel = 0;
		env->output = 0;
		updateStageVariables(env, ATTACK);
	}
	envelope_update(env);
}


#endif
