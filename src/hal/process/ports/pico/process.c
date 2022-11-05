#include <pico/multicore.h>
#include <stdio.h>
#include <stdlib.h>

#include "hal/process.h"

void start_core0(core_loop_fn core_loop) { core_loop(); }

void start_core1(core_loop_fn core_loop) {
  // Launch core 1
  multicore_launch_core1(core_loop);
}
