//
// Created by michael on 1/12/20.
//

#ifndef THE_SYNTH_TEENSY_RING_BUFFER_H
#define THE_SYNTH_TEENSY_RING_BUFFER_H

#include <cstdint>
#include <cstddef>

struct ring_buf_t{
    uint8_t * buffer;
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
};

typedef ring_buf_t* rbuf_handle_t;

/// Pass in a storage buffer and size
/// Returns a circular buffer handle
void ring_buf_init(rbuf_handle_t rbuf, uint8_t* buffer, size_t size);

/// Free a circular buffer structure.
/// Does not free data buffer; owner is responsible for that
void ring_buf_free(rbuf_handle_t rbuf);

/// Reset the circular buffer to empty, head == tail
void ring_buf_reset(rbuf_handle_t rbuf);

/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
void ring_buf_put(rbuf_handle_t rbuf, uint8_t data);

/// Put Version 2 rejects new data if the buffer is full
/// Returns 0 on success, -1 if buffer is full
int ring_buf_put2(rbuf_handle_t rbuf, uint8_t data);

/// Retrieve a value from the buffer
/// Returns 0 on success, -1 if the buffer is empty
int8_t ring_buf_get(rbuf_handle_t rbuf, uint8_t * data);

/// Returns true if the buffer is empty
bool ring_buf_empty(rbuf_handle_t rbuf);

/// Returns true if the buffer is full
bool ring_buf_full(rbuf_handle_t rbuf);

/// Returns the maximum capacity of the buffer
size_t ring_buf_capacity(rbuf_handle_t rbuf);

/// Returns the current number of elements in the buffer
size_t ring_buf_size(rbuf_handle_t rbuf);

#endif //THE_SYNTH_TEENSY_RING_BUFFER_H
