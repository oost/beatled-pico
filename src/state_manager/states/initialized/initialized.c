#include <stdio.h>

#include "command/command.h"
#include "hal/process.h"
#include "initialized.h"
#include "process/core0.h"
#include "process/core1.h"

// bool timer_callback(repeating_timer_t *rt) {

//   return true; // keep repeating
// }

int enter_initialized_state() {
  // Launch core 1

  // start_core1(&core0_entry);
  // start_core0(&core1_entry);
  // // start_core0(&isr_entry);
  // join_cores();
  // start_core1(&core1_entry);
  // // multicore_launch_core1(core1_entry);

  // // Start infinite loop on core 0
  // start_core0(&core0_entry);
  // core0_loop();

  // repeating_timer_t timer;

  // // negative timeout means exact delay (rather than delay between callbacks)
  // if (!add_repeating_timer_us(-1000000, timer_callback, NULL, &timer)) {
  //   printf("Failed to add timer\n");
  //   return 1;
  // }

  // cancel_repeating_timer(&timer);

  return 0;
}
int exit_initialized_state() { return 0; }