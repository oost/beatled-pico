#ifndef UTILS__DATA_QUEUE_H
#define UTILS__DATA_QUEUE_H

#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>

typedef struct {
  void *func;
  int32_t data;
} queue_entry_t;

extern queue_t call_queue;
extern queue_t results_queue;

#endif // !UTILS__DATA_QUEUE_H
