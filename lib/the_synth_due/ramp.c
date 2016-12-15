#include "ramp.h"

static inline void updateStageVariables(struct ramp_struct * ramp, RampStage_t stage){

	switch(stage){

		case RAMP_ATTACK:
			ramp->stageLevel = 0;
			ramp->stageAdd = (ramp->stageLevel * ramp->levelCV) >> 16;
			ramp->stageMul = ((65535 - ramp->stageLevel) * ramp->levelCV )>> 16;
			ramp->stageIncreament=ramp->attackIncreament;
			break;
    case RAMP_SUSTAIN:
			ramp->stageAdd = 0;
			ramp->stageMul = 65535;
			ramp->stageIncreament = 0;
			break;
		default:
			ramp->stageAdd=0;
			ramp->stageMul=0;
			ramp->stageIncreament=0;
	}
}

inline void ramp_trigger(struct ramp_struct * ramp, uint32_t level){
	ramp->output = 0;
	ramp->phase = 0;
  ramp->stageAdd = 0;
	ramp->stage = RAMP_ATTACK;
	ramp->levelCV = level;

	updateStageVariables(ramp, RAMP_ATTACK);
	ramp_update(ramp);
}

static void handlePhaseOverflow(struct ramp_struct * ramp){
	ramp->phase = 0;
	ramp->stageIncreament = 0;

	++ramp->stage;

	switch(ramp->stage){
    case RAMP_SUSTAIN:
      updateStageVariables(ramp, RAMP_SUSTAIN);
      return;
		case RAMP_DEAD:
			ramp->stage=RAMP_DEAD;
			ramp->output = 0;
			updateStageVariables(ramp, RAMP_DEAD);
			return;
		default:
		;

	}
}

inline RampStage_t ramp_getStage(struct ramp_struct * ramp){
	return ramp->stage;
}

inline int32_t ramp_getOutput(struct ramp_struct * ramp)
{
	return ramp->output - ramp->levelCV + 1;
}


void ramp_setup(struct ramp_struct * ramp, uint32_t attack){
	ramp->attackIncreament=attack;
}

inline void ramp_setStage(struct ramp_struct * ramp, RampStage_t stage){
	ramp->stage = stage;
	ramp->phase = 0;
	updateStageVariables(ramp, stage);
}

inline void ramp_update(struct ramp_struct * ramp){

	if(ramp->phase>>20)
		handlePhaseOverflow(ramp);

	uint16_t o = 0;

	switch(ramp->stage){
		case RAMP_ATTACK:
			o = (ramp->phase)>>4;
			break;
    case RAMP_SUSTAIN:
			o = (ramp->levelCV);
			break;
		default:
			ramp->stageAdd = 0
		;
	}

	ramp->output = ((o * ramp->stageMul) >> 16) + ((ramp->stageAdd)>>8);

	ramp->phase+=ramp->stageIncreament;
}
