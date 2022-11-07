#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "event/event_queue.h"
#include "hal/process.h"
#include "process/core0.h"
#include "process/core1.h"
#include "process/isr_thread.h"
#include "state_manager/state.h"
#include "state_manager/state_manager.h"

int main(void) {
  puts("Starting beatled");

  puts("- Starting Event queue");
  event_queue_init();

  puts("- Starting State Manager");
  state_manager_init();

  if (state_manager_set_state(STATE_STARTED) != 0) {
    return 1;
  }

  start_core1(&core1_entry);
  start_isr_thread(&isr_thread_entry);

  core0_entry(NULL);

  join_cores();

  return 0;
}
