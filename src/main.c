#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "event/event_queue.h"
#include "hal/process.h"
#include "process/core0.h"
#include "state_manager.h"

#include "hal/startup.h"

int main(void) {
  startup();

  puts("Starting beatled.");

  puts("- Starting State Manager");
  state_manager_init();

  if (state_manager_set_state(STATE_STARTED) != 0) {
    return 1;
  }

  core0_entry(NULL);

  join_cores();

  return 0;
}
