/*
 * Envelope.c
 *
 * Created: 2/12/2016 9:25:22 PM
 *  Author: Michael Haines
 */
#include "Envelope.h"

inline void envelope_trigger(struct envelope_struct * env, uint32_t level){
	env->output = 0;
	env->phase = 0;

	env->stage = ATTACK;
	env->levelCV = level;

	updateStageVariables(env, ATTACK);
	envelope_update(env);
}

static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage){

	switch(stage){

		case ATTACK:
			envelope->stageLevel = 0;
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
			envelope->levelCV = ((uint16_t)envelope->output<<8);
			envelope->stageAdd = 0;
			envelope->stageMul = ((65535 - envelope->stageLevel) * envelope->levelCV )>> 16;
			envelope->stageIncreament=envelope->releaseIncreament;
			break;
		default:
			envelope->stageAdd=0;
			envelope->stageMul=0;
			envelope->stageIncreament=0;
	}
}

static void handlePhaseOverflow(struct envelope_struct * envelope){
	envelope->phase = 0;
	envelope->stageIncreament = 0;

	++envelope->stage;

	switch(envelope->stage){

		case DECAY:
			envelope->output=envelope->levelCV;
			updateStageVariables(envelope, DECAY);
			return;
		case SUSTAIN:
			updateStageVariables(envelope, SUSTAIN);
			return;
		case DEAD:
			envelope->stage=DEAD;
			envelope->output = 0;
			updateStageVariables(envelope, DEAD);
			return;
		default:
		;

	}
}

inline EnvelopeStage_t env_getStage(struct envelope_struct * envelope){
	return envelope->stage;
}

inline uint16_t env_getOutput(struct envelope_struct * envelope)
{
	return envelope->output;
}


void envelope_setup(struct envelope_struct * envelope, uint32_t attack, uint32_t decay, uint16_t sustain, uint32_t release){
	envelope->attackIncreament=attack;
	envelope->decayIncreament=decay;
	envelope->sustainCV=sustain;
	envelope->releaseIncreament=release;
}

inline void envelope_setStage(struct envelope_struct * envelope, EnvelopeStage_t stage){
	envelope->stage = stage;
	envelope->phase = 0;
	updateStageVariables(envelope, stage);
}

inline void envelope_update(struct envelope_struct * env){

	if(env->phase>>20)
		handlePhaseOverflow(env);

	uint16_t o = 0;

	switch(env->stage){
		case ATTACK:
			o = (env->phase)>>12;
			break;
		case DECAY:
		case RELEASE:
			o = 255 - ((env->phase)>>12);
			break;
		case SUSTAIN:
			o = (env->sustainCV)>>8;
			break;
		default:
			env->stageAdd = 0
		;
	}

	env->output = ((o * env->stageMul) >> 16) + ((env->stageAdd)>>8);

	env->phase+=env->stageIncreament;
}
