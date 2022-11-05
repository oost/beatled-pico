#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "circular_buffer.h"

/// This implementation is threadsafe for a single producer and single consumer

// The definition of our circular buffer structure is hidden from the user
struct hal_queue_t {
  uint8_t *data;
  unsigned int element_count; // of the buffer
  size_t element_size;
  size_t head;
  size_t tail;
};

#pragma mark - Private Functions -

static inline unsigned int advance_headtail_value(unsigned int value,
                                                  unsigned int element_count) {
  if (++value > element_count) {
    value = 0;
  }

  return value;
}

static inline void *element_ptr(hal_queue_handle_t q, unsigned int index) {
  assert(index <= q->element_count);
  return q->data + index * q->element_size;
}

#pragma mark - APIs -

hal_queue_handle_t circular_buf_init(unsigned int element_count,
                                     size_t element_size) {
  assert(element_size > 0);

  hal_queue_handle_t cbuf = malloc(sizeof(hal_queue_t));
  assert(cbuf);

  cbuf->data = calloc(element_count, element_size);
  cbuf->element_count = element_count;
  circular_buf_reset(cbuf);

  assert(circular_buf_empty(cbuf));

  return cbuf;
}

void circular_buf_free(hal_queue_handle_t me) {
  assert(me);
  free(me->data);
  free(me);
}

void circular_buf_reset(hal_queue_handle_t me) {
  assert(me);

  me->head = 0;
  me->tail = 0;
}

unsigned int circular_buf_size(hal_queue_handle_t me) {
  assert(me);

  // We account for the space we can't use for thread safety
  unsigned int size = me->element_count;

  if (!circular_buf_full(me)) {
    if (me->head >= me->tail) {
      size = (me->head - me->tail);
    } else {
      // off by one?
      size = (me->element_count + me->head - me->tail);
    }
  }

  return size;
}

unsigned int circular_buf_capacity(hal_queue_handle_t me) {
  assert(me);

  // We account for the space we can't use for thread safety
  return me->element_count;
}

/// For thread safety, do not use put - use try_put.
/// Because this version, which will overwrite the existing contents
/// of the buffer, will involve modifying the tail pointer, which is also
/// modified by get.
void circular_buf_put(hal_queue_handle_t me, uint8_t data) {
  assert(me && me->data);

  me->data[me->head * me->element_size] = data;
  if (circular_buf_full(me)) {
    // THIS CONDITION IS NOT THREAD SAFE
    me->tail = advance_headtail_value(me->tail, me->element_count);
  }

  me->head = advance_headtail_value(me->head, me->element_count);
}

int circular_buf_try_put(hal_queue_handle_t me, uint8_t data) {
  assert(me && me->data);

  int r = -1;

  if (!circular_buf_full(me)) {
    me->data[me->head * me->element_size] = data;
    me->head = advance_headtail_value(me->head, me->element_count);
    r = 0;
  }

  return r;
}

int circular_buf_get(hal_queue_handle_t me, uint8_t *data) {
  assert(me && data && me->data);

  int r = -1;

  if (!circular_buf_empty(me)) {
    *data = me->data[me->tail * me->element_size];
    me->tail = advance_headtail_value(me->tail, me->element_count);
    r = 0;
  }

  return r;
}

bool circular_buf_empty(hal_queue_handle_t me) {
  assert(me);
  return me->head == me->tail;
}

bool circular_buf_full(hal_queue_handle_t me) {
  // We want to check, not advance, so we don't save the output here
  return advance_headtail_value(me->head, me->element_count) == me->tail;
}

int circular_buf_peek(hal_queue_handle_t me, uint8_t *data,
                      unsigned int look_ahead_counter) {
  int r = -1;
  size_t pos;

  assert(me && data && me->data);

  // We can't look beyond the current buffer size
  if (circular_buf_empty(me) || look_ahead_counter > circular_buf_size(me)) {
    return r;
  }

  pos = me->tail;
  for (unsigned int i = 0; i < look_ahead_counter; i++) {
    data[i] = me->data[pos * me->element_size];
    pos = advance_headtail_value(pos, me->element_count);
  }

  return 0;
}