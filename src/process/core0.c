#include <stdio.h>
#include <stdlib.h>

#include "autotest.h"
#include "command.h"
#include "constants.h"
#include "core0.h"
#include "event/event_loop.h"
#include "hal/blink.h"
#include "hal/process.h"
#include "state_manager/state_manager.h"

void *core0_entry(void *data) {
  core0_init();
  core0_loop();
  return NULL;
}

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

  send_hello_request();

  event_t event;

#ifdef PICO_AUTOTEST
  test_tempo();
#endif // PICO_AUTOTEST

  run_event_loop(&handle_event);
}
