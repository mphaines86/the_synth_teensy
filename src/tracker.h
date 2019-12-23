//
// Created by michael on 12/21/19.
//

#ifndef THE_SYNTH_TEENSY_TRACKER_H
#define THE_SYNTH_TEENSY_TRACKER_H

#include <cstdint>
#include "synth.h"

#define TRACKER_POINTS 4
#define TRACKER_SLOPES TRACKER_POINTS - 1

struct track_struct {
    uint16_t points[TRACKER_POINTS];
    uint16_t notes[TRACKER_POINTS];
    int32_t slope[TRACKER_POINTS - 1];
    int32_t y_intercept[TRACKER_POINTS - 1];
    int16_t value[SYNTH_VOICE_COUNT];
    uint16_t level;
};


void trackerSetNoteLimit(track_struct *track, uint8_t point, uint8_t note);
void trackerSetNoteLimit(track_struct *track, uint8_t note[TRACKER_POINTS]);
void trackerSetPoint(track_struct *track, uint8_t point, uint8_t value);
void trackerSetValue(track_struct *track, uint8_t note, uint8_t voice, uint16_t value);

inline uint16_t trackerGetValue(track_struct * track, uint8_t voice, uint16_t value){
    return value + track->value[voice];
}
#endif //THE_SYNTH_TEENSY_TRACKER_H
