#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "event/event_queue.h"
#include "hal/process.h"
#include "process/core0.h"
#include "state_manager.h"

#include "hal/startup.h"

void start_beatled() {

  puts("Starting beatled.");

  puts("- Starting State Manager");
  state_manager_init();

  if (state_manager_set_state(STATE_STARTED) != 0) {
    return;
  }

  core0_entry(NULL);

  join_cores();
}

int main(void) {
  startup(&start_beatled);

  return 0;
}
