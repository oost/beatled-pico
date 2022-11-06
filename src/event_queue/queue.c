// #include <pico/util/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_queue/queue.h"
#include "hal/queue.h"
#include "hal/time.h"

#define MAX_QUEUE_COUNT 20

// static queue_t event_queue;

hal_queue_handle_t event_queue_ptr;

int add_payload_to_event_queue(void *buffer_payload, size_t size) {
  return event_queue_add_message(event_server_message, buffer_payload, size);
}

void event_queue_init() {
  event_queue_ptr = hal_queue_init(sizeof(event_t), MAX_QUEUE_COUNT);
  // queue_init(&event_queue, sizeof(event_t), MAX_QUEUE_COUNT);
}

int event_queue_add_message(event_type_t event_type, void *event_data,
                            size_t data_length) {
  event_t event;
  event.event_type = event_type;
  event.time = time_us_64();
  event.data = event_data;
  event.data_length = data_length;

  if (!hal_queue_add_message(event_queue_ptr, &event)) {
    puts("Queued is FULL!!!");
    return 1;
  }
  puts("Queued message");
  return 0;
}

int event_queue_pop_message(event_t *event) {
  if (!hal_queue_pop_message(event_queue_ptr, event)) {
    return 1;
  }

  puts("Popped a message from queue");
  return 0;
}