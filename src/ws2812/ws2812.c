/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hardware/clocks.h>
#include <hardware/dma.h>
#include <hardware/irq.h>
#include <hardware/pio.h>
#include <pico/sem.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "state_manager/state_manager.h"
#include "ws2812_config.h"
#include "ws2812_dma.h"
#include "ws2812_patterns.h"

#include "ws2812.pio.h"

static PIO pio;
static uint offset;
static uint sm;
static uint32_t cycle_idx = 0;
static uint8_t pattern_idx = 0;

static uint64_t time_ref = 0;
static uint32_t tempo_period_us = 120 * 1000000 / 60;

pattern *pattern_table;
uint pattern_count;

void led_init() {
  // todo get free sm
  pio = pio0;
  offset = pio_add_program(pio, &ws2812_program);

  get_all_patterns_table(pattern_table, &pattern_count);

  // Find a free state machine on our chosen PIO (erroring if there are
  // none). Configure it to run our program, and start it, using the
  // helper function we included in our .pio file.
  sm = pio_claim_unused_sm(pio, true);

  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
  dma_init(pio, sm);

  printf("Initialized LED driver\n");
}

void led_update_pattern_idx(uint8_t pattern_idx) {
  if (pattern_idx >= count_of(pattern_table)) {
    puts("Index out of range, skipping...");
    return;
  }
  pattern_idx = pattern_idx;
  printf("Updated pattern to: %s\n", pattern_table[pattern_idx].name);
  // puts(dir == 1 ? "(forward)" : "(backward)");
}

void led_set_random_pattern() {
  led_update_pattern_idx(rand() % pattern_count);
}

void led_beat() { cycle_idx = 0; }

void led_update_tempo(uint32_t new_tempo_period_us) {
  tempo_period_us = new_tempo_period_us;
  printf("Updated state tempo: %lu", tempo_period_us);
}

void led_update_time_ref(uint64_t new_time_ref) {
  time_ref = new_time_ref;
  printf("Updated state time_ref: %llu", time_ref);
}

void led_update() {
  if (time_ref == 0) {
    return;
  }

  static uint32_t colors[2][NUM_PIXELS];
  static uint current_stream = 0;
  static uint32_t prev_beat_frac = 0;

  static uint32_t beat_frac;

  int64_t duration_from_time_ref = time_us_64() - time_ref;

  beat_frac =
      (duration_from_time_ref % tempo_period_us) * UINT32_MAX / tempo_period_us;

  if (beat_frac < prev_beat_frac) {
    puts("Beat ... ");
  }

  run_pattern(pattern_idx, colors[current_stream], NUM_PIXELS, beat_frac);

  output_strings_dma(colors[current_stream]);
  current_stream ^= 1;
  prev_beat_frac = beat_frac;

  cycle_idx++;
}
