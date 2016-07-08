/*
 * lfo.c
 *
 * Created: 3/12/2016 11:09:10 PM
 *  Author: Michael Haines
 */
#include "lfo.h"
#include "lfo_tables.h"

void lfo_init(struct lfo_struct * lfo, lfoShape_t shape, uint16_t level, uint16_t rate){
    lfo_setWave(lfo, shape);
    lfo_setRate(lfo, rate);
    lfo->level = level;
}

void lfo_setRate(struct lfo_struct * lfo, uint16_t value){
    lfo->rate= value;
}

void lfo_setWave(struct lfo_struct * lfo, lfoShape_t shape){
  lfo->shape = shape;
    switch (shape){
         case lfoPulse:
            lfo->waveform = square_lfo;
            break;
         case lfoTriangle:
            lfo->waveform = triangle_lfo;
            break;
         case lfoSaw:
            lfo->waveform = saw_lfo;
            break;
         case lfoSine:
            lfo->waveform = sine_lfo;
            break;
         default:
            lfo->waveform = sine_lfo;
            break;
    }
}

int16_t lfo_getOutput(struct lfo_struct * lfo){
    return lfo->output;
}

void lfo_trigger(struct lfo_struct * lfo){
    lfo->phase_accumulators=0;
}

void lfo_update(struct lfo_struct * lfo){
    lfo->frequancy_tuning_word = lfo->rate;
    lfo->phase_accumulators += lfo->frequancy_tuning_word;
    int16_t wave_temp;
    wave_temp = 127 - *(lfo->waveform + ((lfo->phase_accumulators) >> 8));
    lfo->output = ((wave_temp * lfo->level) >> 8);


}
