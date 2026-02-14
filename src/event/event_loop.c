#include <stdio.h>

#include "event/event_loop.h"
#include "event/event_queue.h"
#include "hal/process.h"

void run_event_loop(handle_event_fn handle_event) {
  puts("[INIT] Event loop started");

  event_t event;

  while (1) {
    event_queue_pop_message_blocking(&event);

    if (handle_event(&event)) {
      puts("[ERR] Failed to handle event");
    }
  }
}
