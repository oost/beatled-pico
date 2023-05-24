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
#include "process/intercore_queue.h"
#include "state_manager/state_manager.h"
#include "ws2812/ws2812.h"
#include "ws2812_config.h"
#include "ws2812_patterns.h"

uint32_t _cycle_idx = 0;
uint64_t _time_ref = 0;
uint64_t _last_beat_time = 0;
uint64_t _next_beat_time = 0;
uint64_t _tempo_period_us = 0;
uint8_t _program_id = 0;

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

uint8_t calculate_beat_fraction(uint64_t current_time, uint64_t last_time,
                                uint64_t next_time) {
  return scale8(current_time - last_time, next_time - last_time);
}

uint8_t scale8(uint64_t value, uint64_t range) {
  uint8_t result = 0;
  for (int i = 0; i < 8; i++) {
    range = range >> 1;

    // TODO: is it > or >= ???
    if (value >= range) {
      result += 1 << (7 - i);
      value -= range;
    }
  }

  return result;
}

void advance_next_beat_time(uint64_t current_time) {
  while (_next_beat_time < current_time) {
    puts("Update next beat time");
    _last_beat_time = _next_beat_time;
    _next_beat_time += _tempo_period_us;
  }
}

void update_tempo(intercore_message_t *ic_message) {
  registry_lock_mutex();

  // printf("Message type %d\n", ic_message->message_type);
  if (ic_message->message_type | (0x01 << intercore_time_ref_update)) {
    printf("Update time ref from %llu to %llu. Shift = %llu\n", _next_beat_time,
           registry.next_beat_time_ref,
           registry.next_beat_time_ref - _next_beat_time);

    uint64_t current_beat_duration = _next_beat_time - _last_beat_time;
    _time_ref = registry.next_beat_time_ref;
    // In order to keep the beat fraction constant, shift the last beat time
    _last_beat_time -= (_time_ref - _next_beat_time);
    _next_beat_time = _time_ref;
    uint64_t new_beat_duration = _next_beat_time - _last_beat_time;

    if (new_beat_duration < (current_beat_duration >> 1)) {
      printf("Beat duration is shortened by more than 2, from %llu to %llu\n",
             current_beat_duration, new_beat_duration);
    }
  }

  if (ic_message->message_type | (0x01 << intercore_tempo_update)) {
    // puts("Tempo update");
    _tempo_period_us = registry.tempo_period_us;
  }

  if (ic_message->message_type | (0x01 << intercore_program_update)) {
    // puts("Program update");
    _program_id = registry.program_id;
  }

  registry_unlock_mutex();
}

void led_update() {
  if (_time_ref == 0 || _tempo_period_us == 0) {
    return;
  }

  static uint32_t colors[2][NUM_PIXELS];
  static unsigned int current_stream = 0;
  static uint64_t prev_beat_frac = 0;
  static uint64_t prev_time = 0;

  uint64_t current_time = time_us_64();

  advance_next_beat_time(current_time);

  uint8_t beat_frac =
      calculate_beat_fraction(current_time, _last_beat_time, _next_beat_time);

  run_pattern(_program_id, colors[current_stream], NUM_PIXELS, beat_frac);

  output_strings_dma(colors[current_stream]);
  current_stream ^= 1;

  if (prev_beat_frac > beat_frac) {
    printf("---- BEAT --- %llu, %llu, %llu\n", prev_time, current_time,
           _last_beat_time);
  }

  if (_cycle_idx % 1 == 0) {
    printf("LED cycle %d, program %d, beat_frac %u / %.3f, current_time "
           "%llu, last time %lli, next beat %llu, tempo: %llu\n",
           _cycle_idx, _program_id, beat_frac, (float)beat_frac / UINT8_MAX,
           current_time, _last_beat_time, _next_beat_time, _tempo_period_us);
  }

  prev_beat_frac = beat_frac;
  prev_time = current_time;
  _cycle_idx++;
}
