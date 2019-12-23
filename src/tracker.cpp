//
// Created by michael on 12/21/19.
//

#include "tracker.h"

static inline void trackerCalculateSlope(track_struct * track, uint8_t point){

    if (point == 0){
        track->slope[point] = (track->points[point + 1] - track->points[point]) /
                              (track->notes[point + 1] - track->notes[point]);
        track->y_intercept[point] = track->points[point] - track->slope[point] * track->notes[point];
    }
    else if (point == TRACKER_POINTS-1){
        track->slope[point - 1] = (track->points[point] - track->points[point - 1]) /
                                  (track->notes[point] - track->notes[point - 1]);
        track->y_intercept[point-1] = track->points[point] - track->slope[point - 1] * track->notes[point];
    }
    else{
        track->slope[point] = (track->points[point + 1] - track->points[point]) /
                              (track->notes[point + 1] - track->notes[point]);
        track->slope[point + 1] = (track->points[point] - track->points[point - 1]) /
                                  (track->notes[point] - track->notes[point - 1]);
        track->y_intercept[point] = track->points[point] - track->slope[point] * track->notes[point];
        track->y_intercept[point + 1] = track->points[point] - track->slope[point + 1] * track->notes[point];
    }
}

void trackerSetNoteLimit(track_struct *track, uint8_t point, uint8_t note){
    track->notes[point] = note;
    trackerCalculateSlope(track, point);
}

void trackerSetNoteLimit(track_struct *track, uint8_t note[TRACKER_POINTS]){
    for(uint8_t i = 0; i < TRACKER_POINTS; i++){
        track->notes[i] = note[i];
        trackerCalculateSlope(track, i);
    }
}

void trackerSetPoint(track_struct *track,uint8_t point, uint8_t value){
    track->points[point] = value;
    trackerCalculateSlope(track, point);


}

void trackerSetValue(track_struct *track, uint8_t note, uint8_t voice, uint16_t value){

    int8_t region = -1;
    for(uint8_t i = 0;i<TRACKER_POINTS;i++){
        if(note < track->notes[i]){
            region = i;
            break;
        }
    }

    switch (region){
        case -1:{
            track->value[voice] = (track->points[TRACKER_POINTS-1] * track->level) >> 16;
            break;
        }
        case 0:{
            track->value[voice] = (track->points[0] * track->level) >> 16;
            break;
        }
        default:{
            track->value[voice] = ((track->slope[region] * note + track->y_intercept[region]) * track->level) >> 16;
        }
    }
}

