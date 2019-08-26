/*
 * lfo.c
 *
 * Created: 3/12/2016 11:09:10 PM
 *  Author: Michael Haines
 */
#include "lfo.h"
#include "lfo_tables.h"

void lfo_init(struct lfo_struct *lfo, lfoShape_t shape, uint16_t level, uint16_t rate, uint8_t speed) {
    lfo_setWave(lfo, shape);
    lfo_setRate(lfo, rate >> (speed * 6));
    lfo->level = level;
}

void lfo_setRate(struct lfo_struct * lfo, uint16_t value){
    lfo->rate= value;
}

void lfo_setRate(struct lfo_struct * lfo, uint8_t key, int8_t offset){
    uint16_t pitch = (key - offset) * 240;
    lfo->rate = lfo_CVtoFrequancy(pitch);
}

static uint16_t lfo_CVtoFrequancy(uint16_t cv){
    uint8_t octave = cv / 2880;
    uint8_t pitch_index = (cv * 240) / 2880 - octave * 240;
    uint32_t pre_pitching = One_Octave_Pitches[pitch_index];
    return (pre_pitching>>(10 - octave));
}


void lfo_setLevel(struct lfo_struct * lfo, uint16_t level){
    lfo->level = level;
}

void lfo_setKeyfollow(struct lfo_struct * lfo, boolean key){
    lfo->key_follow = key;
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

/*int16_t lfo_getOutput(struct lfo_struct * lfo){
    return lfo->output;
}*/

void lfo_trigger(struct lfo_struct * lfo){
    lfo->phase_accumulators=0;
}

void lfo_trigger(struct lfo_struct * lfo, uint8_t key, int8_t offset){
    lfo->phase_accumulators=0;
    lfo_setRate(lfo, key, offset);
}

/*void lfo_update(struct lfo_struct * lfo){
    lfo->frequancy_tuning_word = lfo->rate;
    lfo->phase_accumulators += lfo->frequancy_tuning_word;
    int16_t wave_temp;
    wave_temp = *(lfo->waveform + ((lfo->phase_accumulators) >> 8));
    lfo->output = ((wave_temp * lfo->level) >> 8);


}*/
