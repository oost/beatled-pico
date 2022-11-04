#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "core1.h"
#include "intercore_queue.h"
#include "state_manager/state.h"
#include "ws2812/ws2812.h"

void core1_init() { printf("Initializing core 1\n"); }

void update_state(state_update_t *new_state) {
  if (new_state->tempo_time_ref) {
    led_update_time_ref(new_state->tempo_time_ref);
  }
  if (new_state->tempo_period_us) {
    led_update_tempo(new_state->tempo_period_us);
  }
  if (new_state->program_idx) {
    led_update_pattern_idx(new_state->program_idx);
  }
}

void core1_loop() {
  state_update_t new_state;
  printf("Starting core 1 loop\n");

  while (1) {
    if (queue_try_remove(&intercore_command_queue, &new_state)) {
      puts("Updating state in core 1");
      update_state(&new_state);
    }

    led_update();
    sleep_ms(LED_CORE_SLEEP_MS);
  }
}