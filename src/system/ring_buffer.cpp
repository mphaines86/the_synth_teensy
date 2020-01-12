//
// Created by michael on 1/12/20.
//

#include "ring_buffer.h"

static inline void advance_pointer(rbuf_handle_t rbuf){
    if(rbuf->full){
        rbuf->tail = (rbuf->tail + 1) % rbuf->max;
    }

    rbuf->head = (rbuf->head + 1) % rbuf->max;
    rbuf->full = (rbuf->head == rbuf->tail);
}

static inline void retreat_pointer(rbuf_handle_t rbuf){
    rbuf->full = false;
    rbuf->tail = (rbuf->tail + 1) % rbuf->max;
}

void ring_buf_init(rbuf_handle_t rbuf, uint8_t *buffer, size_t size) {
    rbuf->buffer = buffer;
    rbuf->max = size;
    ring_buf_reset(rbuf);
}

void ring_buf_reset(rbuf_handle_t rbuf){
    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->full = false;
}

bool ring_buf_full(rbuf_handle_t rbuf){
    return rbuf->full;
}

bool ring_buf_empty(rbuf_handle_t rbuf){
    return (!rbuf->full && (rbuf->head == rbuf->tail));
}

size_t ring_buf_capacity(rbuf_handle_t rbuf){
    return rbuf->max;
}

size_t ring_buf_size(rbuf_handle_t rbuf){
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

void ring_buf_put(rbuf_handle_t rbuf, uint8_t data){
    rbuf->buffer[rbuf->head] = data;
    advance_pointer(rbuf);
}

int8_t ring_buf_get(rbuf_handle_t rbuf, uint8_t * data){

    int8_t r = -1;

    if(!ring_buf_empty(rbuf)){
        *data = rbuf->buffer[rbuf->tail];
        retreat_pointer(rbuf);

        r = 0;
    }

    return r;

}