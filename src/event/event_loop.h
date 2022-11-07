#ifndef EVENT__EVENT_LOOP_H_
#define EVENT__EVENT_LOOP_H_

#include <stdint.h>

#include "event_queue.h"

typedef int (*handle_event_fn)(event_t *);

void event_loop(uint32_t event_loop_sleep_ms, handle_event_fn handle_event);

#endif // EVENT__EVENT_LOOP_H_