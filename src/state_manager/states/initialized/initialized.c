#include "initialized.h"
#include "command/command.h"
#include "core0.h"
#include "core1.h"

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

  return 0;
}
int exit_initialized_state() { return 0; }