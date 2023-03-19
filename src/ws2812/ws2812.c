/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "hal/registry.h"
#include "hal/ws2812.h"
#include "state_manager/state_manager.h"
#include "ws2812_config.h"
#include "ws2812_patterns.h"

static uint32_t cycle_idx = 0;
// static uint8_t pattern_idx = 0;

void led_init() {
  ws2812_init(NUM_PIXELS, WS2812_PIN, 800000, IS_RGBW);

  printf("Initialized LED manager\n");
}

void led_set_random_pattern() {
  if (registry_try_lock_mutex()) {
    // registry_set_program(rand() % get_pattern_count());
    registry.program_id = 0;
    registry_unlock_mutex();
  }
}

void led_beat() { cycle_idx = 0; }

void led_update() {
  if (registry.next_beat_time_ref == 0 || registry.tempo_period_us == 0) {
    return;
  }

  static uint32_t colors[2][NUM_PIXELS];
  static unsigned int current_stream = 0;

  static uint64_t beat_frac;

  uint64_t current_time = time_us_64();
  while (current_time > registry.next_beat_time_ref) {
    printf("Beat ... current %llu , last %llu , next %llu\n", current_time,
           registry.last_beat_time_ref, registry.next_beat_time_ref);
    registry.last_beat_time_ref = registry.next_beat_time_ref;
    registry.next_beat_time_ref += registry.tempo_period_us;
  }

  uint64_t duration_from_time_ref =
      (current_time - registry.last_beat_time_ref);
  beat_frac = (duration_from_time_ref % registry.tempo_period_us) * UINT32_MAX /
              registry.tempo_period_us;
  // printf("%llu\n", beat_frac);
  run_pattern(registry.program_id, colors[current_stream], NUM_PIXELS,
              (uint32_t)beat_frac);

  output_strings_dma(colors[current_stream]);
  current_stream ^= 1;
  // prev_beat_frac = beat_frac;

  cycle_idx++;
}
