#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_queue/queue.h"

#define MAX_QUEUE_COUNT 20

static queue_t event_queue;

void event_queue_init() {
  queue_init(&event_queue, sizeof(event_t), MAX_QUEUE_COUNT);
}

bool event_queue_add_message(event_type_t event_type, void *event_data,
                             size_t data_length) {
  event_t event;
  event.event_type = event_type;
  event.time = time_us_64();
  event.data = event_data;
  event.data_length = data_length;

  if (!queue_try_add(&event_queue, &event)) {
    puts("Queued is FULL!!!");
    return false;
  }
  puts("Queued message");
  return true;
}

bool event_queue_pop_message(event_t *event) {
  if (!queue_try_remove(&event_queue, event)) {
    return false;
  }

  puts("Popped a message from queue");
  return true;
}