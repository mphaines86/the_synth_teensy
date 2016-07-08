/*
 * lfo.h
 *
 * Created: 3/12/2016 11:08:25 PM
 *  Author: Michael Haines
 */


#ifndef LFO_H_
#define LFO_H_

#include  "synth.h"


typedef enum {
  lfoPulse = 0, lfoTriangle = 1, lfoSaw = 2, lfoSine=3
} lfoShape_t;

struct lfo_struct{
	uint16_t phase_accumulators;
	uint16_t frequancy_tuning_word;
	uint16_t rate;
	int16_t output;
  uint16_t level;
  lfoShape_t shape;

  const uint8_t *waveform;
};

void lfo_init(struct lfo_struct * lfo, lfoShape_t shape, uint16_t level, uint16_t rate);
void lfo_trigger(struct lfo_struct * lfo);
void lfo_setRate(struct lfo_struct * lfo, uint16_t value);
void lfo_setWave(struct lfo_struct * lfo, lfoShape_t shape);
int16_t lfo_getOutput(struct lfo_struct * lfo);
void lfo_update(struct lfo_struct * lfo);

#endif
