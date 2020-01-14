//
// Created by michael on 1/12/20.
//

#include <Arduino.h>
#include "ring_buffer.h"

static inline void advance_pointer(ring_buffer_t *rbuf){
    if(rbuf->full){
        rbuf->tail = (rbuf->tail + 1) % rbuf->max;
    }

    rbuf->head = (rbuf->head + 1) % rbuf->max;
    rbuf->full = (rbuf->head == rbuf->tail);
}

static inline void retreat_pointer(ring_buffer_t *rbuf){
    rbuf->full = false;
    rbuf->tail = (rbuf->tail + 1) % rbuf->max;
}

void ring_buffer_init(ring_buffer_t *rbuf, uint16_t *buffer, size_t size) {
    rbuf->buffer = buffer;
    rbuf->max = size/2;
    ring_buffer_reset(rbuf);
}

void ring_buffer_reset(ring_buffer_t *rbuf){
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->full = false;
}

bool ring_buffer_full(ring_buffer_t *rbuf){
    return rbuf->full;
}

bool ring_buffer_empty(ring_buffer_t *rbuf){
    return (!rbuf->full && (rbuf->head == rbuf->tail));
}

size_t ring_buffer_capacity(ring_buffer_t *rbuf){
    return rbuf->max;
}

size_t ring_buffer_size(ring_buffer_t *rbuf){
    size_t size = rbuf->max;

    if(!rbuf->full){
        if(rbuf->head >= rbuf->tail){
            size = (rbuf->head - rbuf->tail);
        }
        else{
            size = (rbuf->max + rbuf->head - rbuf->tail);
        }
    }

    return size;
}

void ring_buffer_put(ring_buffer_t *rbuf, uint16_t data){
    //Serial.println(rbuf->max);
    rbuf->buffer[rbuf->head] = data;
    advance_pointer(rbuf);
}

uint16_t ring_buffer_get(ring_buffer_t *rbuf) {

    uint16_t data = 0;

    if(!ring_buffer_empty(rbuf)){
        data = rbuf->buffer[rbuf->tail];
        retreat_pointer(rbuf);

        //data = 0;
    }

    return data;

}