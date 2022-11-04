#include "hal/multiprocessing.h"
#include <stdio.h>

#include "command/command.h"
#include "core0.h"
#include "core1.h"
#include "initialized.h"

bool timer_callback(repeating_timer_t *rt) {

  return true; // keep repeating
}

void core1_entry() {
  core1_init();
  core1_loop();
}

int enter_initialized_state() {
  // Launch core 1
  multicore_launch_core1(core1_entry);

  // Do core 0 specific initialization
  core0_init();

  // Start infinite loop on core 0
  core0_loop();
  send_hello_request();

  repeating_timer_t timer;

  // negative timeout means exact delay (rather than delay between callbacks)
  if (!add_repeating_timer_us(-1000000, timer_callback, NULL, &timer)) {
    printf("Failed to add timer\n");
    return 1;
  }

  cancel_repeating_timer(&timer);

  return 0;
}
int exit_initialized_state() { return 0; }