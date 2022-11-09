#include <stdio.h>

#include "command/command.h"
#include "hal/process.h"
#include "initialized.h"
#include "process/core0.h"
#include "process/core1.h"
#include "process/isr_thread.h"

int enter_initialized_state() {
  // Launch core 1
  start_core1(&core1_entry);
  start_isr_thread(&isr_thread_entry);

  return 0;
}
int exit_initialized_state() { return 0; }