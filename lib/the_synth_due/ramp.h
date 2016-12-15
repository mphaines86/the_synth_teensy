
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

void ramp_trigger(struct ramp_struct * ramp, uint32_t level);
void ramp_setStage(struct ramp_struct * ramp, RampStage_t stage);
void ramp_setup(struct ramp_struct * ramp, uint32_t attack);
int32_t ramp_getOutput(struct ramp_struct * ramp);
RampStage_t ramp_getStage(struct ramp_struct * ramp);
void ramp_update(struct ramp_struct * ramp);


#endif
