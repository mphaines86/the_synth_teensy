//
// Created by michael on 1/12/20.
//

#ifndef THE_SYNTH_TEENSY_RING_BUFFER_H
#define THE_SYNTH_TEENSY_RING_BUFFER_H

#include <cstdint>
#include <cstddef>

struct ring_buffer_t{
    uint16_t * buffer;
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
};

//typedef ring_buffer_t* rbuf_handle_t;

/// Pass in a storage buffer and size
/// Returns a circular buffer handle
void ring_buffer_init(ring_buffer_t *rbuf, uint16_t *buffer, size_t size);

/// Free a circular buffer structure.
/// Does not free data buffer; owner is responsible for that
//void ring_buf_free(rbuf_handle_t rbuf);

/// Reset the circular buffer to empty, head == tail
void ring_buffer_reset(ring_buffer_t *rbuf);

/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
void ring_buffer_put(ring_buffer_t *rbuf, uint16_t data);

/// Put Version 2 rejects new data if the buffer is full
/// Returns 0 on success, -1 if buffer is full
//int ring_buf_put2(rbuf_handle_t rbuf, uint8_t data);

/// Retrieve a value from the buffer
/// Returns 0 on success, -1 if the buffer is empty
uint16_t ring_buffer_get(ring_buffer_t *rbuf);

/// Returns true if the buffer is empty
bool ring_buffer_empty(ring_buffer_t *rbuf);

/// Returns true if the buffer is full
bool ring_buffer_full(ring_buffer_t *rbuf);

/// Returns the maximum capacity of the buffer
size_t ring_buffer_capacity(ring_buffer_t *rbuf);

/// Returns the current number of elements in the buffer
size_t ring_buffer_size(ring_buffer_t *rbuf);

#endif //THE_SYNTH_TEENSY_RING_BUFFER_H
