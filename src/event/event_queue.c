// #include <pico/util/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_queue.h"
#include "hal/queue.h"
#include "hal/time.h"

#define MAX_QUEUE_COUNT 20

// static queue_t event_queue;

hal_queue_handle_t event_queue_ptr;

int add_payload_to_event_queue(void *buffer_payload, size_t size) {
  if (event_queue_add_message(event_server_message, buffer_payload, size)) {
    return 0;
  }
  return 1;
}

void event_queue_init() {
  event_queue_ptr = hal_queue_init(sizeof(event_t), MAX_QUEUE_COUNT);
}

bool event_queue_add_message(event_type_t event_type, void *event_data,
                             size_t data_length) {
  event_t event;
  event.event_type = event_type;
  event.time = time_us_64();
  event.data = event_data;
  event.data_length = data_length;

  return hal_queue_add_message(event_queue_ptr, &event);
}

void event_queue_pop_message_blocking(event_t *event) {
  hal_queue_pop_message_blocking(event_queue_ptr, event);
}