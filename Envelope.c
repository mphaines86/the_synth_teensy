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
	env->attackIncreament = 10;
	env->decayIncreament = 10;
	env->releaseIncreament = 10;
	env->sustainCV = 65533;
	env->levelCV = 65533;
	env->stageIncreament = 10;
	//******************************
	
	updateStageVariables(env, ATTACK);
	envelope_update(env);
}

static inline void updateStageVariables(struct envelope_struct * envelope, EnvelopeStage_t stage){
	
	switch(stage){
		
		case ATTACK:
			envelope->stageIncreament=envelope->attackIncreament;
			break;
		case DECAY:
			envelope->stageIncreament=envelope->decayIncreament;
			break;
		case SUSTAIN:
			envelope->stageIncreament = 0;
			break;
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

inline void envelope_update(struct envelope_struct * env){
	
	if(env->phase>>8)
		handlePhaseOverflow(env);
  
	uint8_t o = 0;
	
	switch(env->stage){
		case ATTACK:
			o = 255 - (env->phase)>>8;
			break;
		case DECAY:
		case RELEASE:
			o = (env->phase)>>8;
			break;
		case SUSTAIN:
			o = env ->sustainCV;
			break;
		default:
		;
	}
	
	env->output = o + env->stageAdd;
	
	env->phase+=env->stageIncreament;
}
