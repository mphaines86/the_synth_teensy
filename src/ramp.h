
/*
 * ramp.h
 *
 * Created: 2/12/2016 9:25:47 PM
 *  Author: Michael Haines
 */


#ifndef ramp_H
#define ramp_H

#include "synth.h"

typedef enum {
	RAMP_ATTACK = 0,
    RAMP_SUSTAIN = 1,
	RAMP_DEAD = 2,
	RAMP_NUM_SEGMENTS,
} RampStage_t;

struct ramp_struct {
	uint32_t stageIncreament;
	uint32_t phase;
	uint32_t attackIncreament, decayIncreament, releaseIncreament;

	uint16_t levelCV, sustainCV;
	uint16_t attackCV;
	uint16_t stageLevel,stageAdd,stageMul;
	uint16_t speedShift;
	uint16_t output;

	RampStage_t stage;
	};

//inline void ramp_trigger(struct ramp_struct * ramp, uint32_t level);
static inline void updateStageVariables(struct ramp_struct * ramp, RampStage_t stage);
void ramp_setStage(struct ramp_struct * ramp, RampStage_t stage);
void ramp_setup(struct ramp_struct * ramp, uint32_t attack);
//inline int32_t ramp_getOutput(struct ramp_struct * ramp);
//inline RampStage_t ramp_getStage(struct ramp_struct * ramp);
void ramp_update(struct ramp_struct * ramp);

inline uint16_t ramp_getOutput(struct ramp_struct *ramp)
{
	return ramp->output;
}


inline RampStage_t ramp_getStage(struct ramp_struct * ramp){
	return ramp->stage;
}


static inline void updateStageVariables(struct ramp_struct * ramp, RampStage_t stage){

	switch(stage){

		case RAMP_ATTACK:
			ramp->stageAdd = (ramp->stageLevel * ramp->levelCV) >> 16;
			ramp->stageMul = ((UINT16_MAX - ramp->stageLevel) * ramp->levelCV )>> 16;
			ramp->stageIncreament=ramp->attackIncreament;
			break;
		case RAMP_SUSTAIN:
			ramp->stageAdd = 0;
			ramp->stageMul = UINT16_MAX;
			ramp->stageIncreament = 0;
			break;
		default:
			ramp->stageAdd=0;
			ramp->stageMul=0;
			ramp->stageIncreament=0;
	}
}

inline void ramp_trigger(struct ramp_struct * ramp, uint16_t rate){
	ramp->output = 0;
	ramp->phase = 0;
	ramp->stageLevel = 0;
	ramp->stage = RAMP_ATTACK;
	ramp->attackIncreament = rate;

	updateStageVariables(ramp, RAMP_ATTACK);
	ramp_update(ramp);
}

#endif
