#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "core1.h"
#include "hal/process.h"
#include "intercore_queue.h"
#include "registry.h"
#include "ws2812.h"
#include "ws2812_patterns.h"

void *core1_entry(void *data) {
  core1_init();
  core1_loop();
  return NULL;
}

void core1_init() {
  printf("Initializing core 1\n");
  registry_init();
}

void update_state(registry_update_t *update_values) {
  if (update_values->registry_update_fields & (0x01 << REGISTRY_UPDATE_TEMPO)) {
    registry_set_tempo(update_values->tempo_time_ref,
                       update_values->tempo_period_us,
                       update_values->update_timestamp);
  }
  if (update_values->registry_update_fields &
      (0x01 << REGISTRY_UPDATE_PROGRAM)) {
    registry_set_program(update_values->program_idx);

    printf("Updated pattern to: %s\n",
           pattern_get_name(update_values->program_idx));
  }

  // if (new_state->tempo_time_ref) {
  //   led_update_time_ref(new_state->tempo_time_ref);
  // }
  // if (new_state->tempo_period_us) {
  //   led_update_tempo(new_state->tempo_period_us);
  // }
  // if (new_state->program_idx) {
  //   led_update_pattern_idx(new_state->program_idx);
  // }
}

void core1_loop() {
  registry_update_t new_state;
  printf("Starting core 1 loop\n");

  while (1) {
    if (hal_queue_pop_message(intercore_command_queue, &new_state)) {
      puts("Updating state in core 1");
      update_state(&new_state);
    }

    led_update();
    sleep_ms(LED_CORE_SLEEP_MS);
  }
}