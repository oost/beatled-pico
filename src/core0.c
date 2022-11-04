#include <stdio.h>
#include <stdlib.h>

#include "autotest/autotest.h"
#include "command/command.h"
#include "constants.h"
#include "core0.h"
#include "hal/blink/blink.h"
#include "state_manager/state_manager.h"

void core0_init() {
  puts("Initializing core 0");

#ifdef PICO_AUTOTEST
  puts("**** Running with autotest *********");
  init_test();
#endif // PICO_AUTOTEST
}

void core0_loop() {
  sleep_ms(2000);
  printf("Starting core 0 loop\n");
  blink(MESSAGE_BLINK_SPEED, MESSAGE_WELCOME);
  sleep_ms(1000);

  state_manager_set_state(STATE_STARTED);

  event_t event;

#ifdef PICO_AUTOTEST
  test_tempo();
#endif // PICO_AUTOTEST

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

    sleep_ms(CONTROL_CORE_SLEEP_MS);
  }
}
