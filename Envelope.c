/*
 * CFile1.c
 *
 * Created: 2/12/2016 9:25:22 PM
 *  Author: Michael Haines
 */ 
#include "Envelope.h"

inline void envelope_trigger(struct envelope_struct * env){
	env->output = 0;
	env->phase = 0;
	
	//These values are only temporary
	env->stageLevel = 0;
	env->stage = ATTACK;
	env->attackIncreament = 100;
	env->decayIncreament = 100;
	env->releaseIncreament = 26;
	env->sustainCV = 65535;
	env->levelCV = 255;
	env->stageIncreament = 100;
	//******************************
	
	updateStageVariables(env, ATTACK);
	envelope_update(env);
}

static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage){
	
	switch(stage){
		
		case ATTACK:
			envelope->stageAdd = 128;
			envelope->stageIncreament=envelope->attackIncreament;
			break;
		case DECAY:
			envelope->stageAdd = envelope->sustainCV;
			envelope->stageIncreament=envelope->decayIncreament;
			break;
		case SUSTAIN:
			envelope->stageAdd = 0;
			envelope->stageIncreament = 0;
			break;
		case RELEASE:
			envelope->stageAdd = 0;
			envelope->stageIncreament=envelope->releaseIncreament;
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

inline void envelope_setStage(struct envelope_struct * envelope, EnvelopeStage_t stage){
	envelope->stage = stage;
	envelope->phase = 0;
	updateStageVariables(envelope, stage);
}

inline void envelope_update(struct envelope_struct * env){
	
	if(env->phase>>16)
		handlePhaseOverflow(env);
  
	uint8_t o = 0;
	
	switch(env->stage){
		case ATTACK:
			o = (env->phase)>>8;
			break;
		case DECAY:
		case RELEASE:
			o = 255 - (env->phase)>>8;
			break;
		case SUSTAIN:
			o = (env->sustainCV)>>8;
			break;
		default:
		;
	}
	
	env->output = o | env->stageAdd;
	
	env->phase+=env->stageIncreament;
}
