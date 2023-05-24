#include <stdio.h>

#include "event/event_loop.h"
#include "event/event_queue.h"
#include "hal/process.h"

void run_event_loop(handle_event_fn handle_event) {
  printf("Starting event loop\n");

  event_t event;

  while (1) {
    event_queue_pop_message_blocking(&event);

    // puts("Got a message, going to parse it...");

    if (handle_event(&event)) {
      puts("Error parsing command :-(");
    }
  }
}
