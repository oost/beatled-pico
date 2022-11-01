#ifndef MANAGER_H
#define MANAGER_H

#include <pico/stdlib.h>

typedef enum { event_error = 0, event_server_message } event_type_t;

typedef struct {
  event_type_t event_type;
  uint64_t time;
  size_t data_length;
  void *data;
} event_t;

void event_queue_init();

bool event_queue_add_message(event_type_t event_type, void *event_data,
                             size_t data_length);
bool event_queue_pop_message(event_t *event);

#endif // MANAGER_H