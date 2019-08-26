/*
 * lfo.h
 *
 * Created: 3/12/2016 11:08:25 PM
 *  Author: Michael Haines
 */


#ifndef LFO_H_
#define LFO_H_

//#include  "synth.h"
#include "Arduino.h"

#define LFO_LENGTH 262144


typedef enum {
  lfoPulse = 0, lfoTriangle = 1, lfoSaw = 2, lfoSine=3
} lfoShape_t;

struct lfo_struct{
	uint32_t phase_accumulators;
	uint32_t frequancy_tuning_word;
	uint32_t rate;
	int64_t output;
    uint16_t level;
    lfoShape_t shape;
    uint8_t key_follow;

    const uint8_t *waveform;
};

static uint16_t lfo_CVtoFrequancy(uint16_t cv);
void lfo_init(struct lfo_struct *lfo, lfoShape_t shape, uint16_t level, uint16_t rate, uint8_t speed);
void lfo_trigger(struct lfo_struct * lfo);
void lfo_trigger(struct lfo_struct * lfo, uint8_t key, int8_t offset);
void lfo_setRate(struct lfo_struct * lfo, uint16_t value);
void lfo_setRate(struct lfo_struct * lfo, uint8_t key, int8_t offset);
void lfo_setWave(struct lfo_struct * lfo, lfoShape_t shape);
void lfo_setLevel(struct lfo_struct * lfo, uint16_t level);
//int16_t lfo_getOutput(struct lfo_struct * lfo);
void lfo_update(struct lfo_struct * lfo);

inline int64_t lfo_getOutput(struct lfo_struct * lfo){
	return lfo->output;
}

inline boolean lfo_getKeyfollow(struct lfo_struct *lfo){
    return lfo->key_follow;
}

inline void lfo_update(struct lfo_struct * lfo){
    lfo->frequancy_tuning_word = lfo->rate;
    lfo->phase_accumulators += lfo->frequancy_tuning_word;
    lfo->phase_accumulators%=LFO_LENGTH;
    //int64_t wave_temp;
    // wave_temp = *(lfo->waveform + ((lfo->phase_accumulators) >> 8));
    lfo->output = ((*(lfo->waveform + ((lfo->phase_accumulators) >> 9)) * lfo->level) >> 8);

}
#endif
