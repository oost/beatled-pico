#include <pico/stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "core1.h"

void core1_init() { printf("Initializing core 1\n"); }

void core1_loop() {
  printf("Starting core 1 loop\n");

  while (1) {
    tight_loop_contents();
  }
}