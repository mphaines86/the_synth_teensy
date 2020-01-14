/*
 * Envelope.c
 *
 * Created: 2/12/2016 9:25:22 PM
 *  Author: Michael Haines
 */
#include "Envelope.h"

/*inline void envelope_trigger(struct envelope_struct * env, uint32_t level){
	env->output = 0;
	env->phase = 0;

	env->stage = ATTACK;
	env->levelCV = level;

	updateStageVariables(env, ATTACK);
	envelope_update(env);
}*/

static void handlePhaseOverflow(struct envelope_struct * envelope){
	envelope->phase = 0;
	envelope->stageIncreament = 0;

	auto stage = static_cast<uint8_t>(envelope->stage) + 1;

	switch(stage){

		case DECAY:
		    envelope->stage = DECAY;
			envelope->output=envelope->levelCV;
			updateStageVariables(envelope, DECAY);
			return;
		case SUSTAIN:
		    envelope->stage = SUSTAIN;
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

/*inline EnvelopeStage_t env_getStage(struct envelope_struct * envelope){
	return envelope->stage;
}*/

/*inline uint16_t env_getOutput(struct envelope_struct * envelope)
{
	return envelope->output;
}*/


void envelope_setup(struct envelope_struct *envelope, uint32_t attack, uint32_t decay, uint16_t sustain,
                    uint32_t release,
                    uint8_t speed, uint8_t trigger, uint16_t amp) {
	uint8_t shifter = speed * 4 + 18;
    envelope->attackIncreament=attack*attack >> shifter;
	envelope->decayIncreament=decay*decay >> shifter;
	envelope->sustainCV=sustain;
	envelope->releaseIncreament=release*release >> shifter;
	envelope->trigger=trigger;
	envelope->amplitude=amp;
}

void envelope_setStage(struct envelope_struct * envelope, EnvelopeStage_t stage){
	envelope->stage = stage;
	envelope->phase = 0;
	updateStageVariables(envelope, stage);
}

void envelope_setGate(struct envelope_struct * envelope, uint8_t gate){
	envelope->phase=0;
	envelope->stageLevel = (uint16_t)((envelope->output<<24)/envelope->levelCV);

	if (gate){
		envelope->stage=ATTACK;
		updateStageVariables(envelope,ATTACK);
	}
	else{
		envelope->stage=RELEASE;
		updateStageVariables(envelope, RELEASE);
	}
}

void envelope_update(struct envelope_struct * env){

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

	env->output = ((((o * env->stageMul) >> 16) + ((env->stageAdd)>>8)) * env->amplitude) >> 16;
	env->phase+=env->stageIncreament;

	ring_buffer_put(&env->ringBuffer, env->output);
}
