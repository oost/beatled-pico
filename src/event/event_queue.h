#ifndef EVENT__EVENT_QUEUE_H
#define EVENT__EVENT_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

#include "event.h"

int add_payload_to_event_queue(void *buffer_payload, size_t size);

void event_queue_init();

bool event_queue_add_message(event_type_t event_type, void *event_data,
                             size_t data_length);
bool event_queue_pop_message(event_t *event);

#endif // EVENT__EVENT_QUEUE_H