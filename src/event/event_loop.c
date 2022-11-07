#include <stdio.h>

#include "event_loop.h"
#include "event_queue.h"
#include "hal/process.h"

void event_loop(uint32_t event_loop_sleep_ms, handle_event_fn handle_event) {
  printf("Starting event loop\n");

  event_t event;

  while (1) {
    while (1) {
      if (!event_queue_pop_message(&event)) {
        break;
      };

      puts("Got a message, going to parse it...");

      if (handle_event(&event)) {
        puts("Error parsing command :-(");
      }
    }

    // sleep_ms(CONTROL_CORE_SLEEP_MS);
    sleep_ms(event_loop_sleep_ms);
  }
}
