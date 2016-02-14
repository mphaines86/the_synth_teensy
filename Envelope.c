/*
 * CFile1.c
 *
 * Created: 2/12/2016 9:25:22 PM
 *  Author: Michael Haines
 */ 
#include "Envelope.h"

inline void trigger(struct envelope_struct * a){
	
}

inline void handlePhaseOverflow(struct envelope_struct * envelope){
	
}

inline EnvelopeStage_t env_getStage(struct envelope_struct * envelope){
	return envelope->stage;
}

inline void envelope_update(struct envelope_struct * a){
	
	if(a->phase>>8)
		handlePhaseOverflow(a);
	
	switch(a->stage){
		case ATTACK:
			break;
		case DECAY:
			break;
		case RELEASE:
			break;
		case SUSTAIN:
			break;
		default:
		;
	}
}