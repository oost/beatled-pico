#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "registry.h"

// typedef struct registry {
//   uint64_t time_offset;
//   uint64_t time_offset_update_timestamp;
//   uint64_t tempo_time_ref;
//   uint32_t tempo_period_us;
//   uint64_t tempo_time_update_timestamp;
//   uint8_t program_idx;
// } registry_t;

registry_t registry;

void registry_init() {
  memset(&registry, 0, sizeof(registry));
  registry.tempo_period_us = 120 * 1000000 / 60; // Default to 120Hz
}

void registry_set_time_offset(uint64_t time_offset, uint64_t update_timestamp) {
  registry.time_offset = time_offset;
  registry.time_offset_update_timestamp = update_timestamp;
}

void registry_set_tempo(uint64_t tempo_time_ref, uint32_t tempo_period_us,
                        uint64_t update_timestamp) {
  registry.tempo_time_ref = tempo_time_ref;
  registry.tempo_period_us = tempo_period_us;
  registry.tempo_time_update_timestamp = update_timestamp;
  printf("Updated state tempo: %u\n", tempo_period_us);
  printf("Updated state time_ref: %llu\n", tempo_time_ref);
}

void registry_set_program(uint8_t program_idx) {
  registry.program_idx = program_idx;
}
