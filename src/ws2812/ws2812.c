/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/sem.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "hardware/clocks.h"

#include "ws2812_config.h"
#include "ws2812_patterns.h"
#include "ws2812_dma.h"

#include "ws2812.pio.h"


static uint32_t colors[2][NUM_PIXELS];

absolute_time_t time_ref;
const float bpm = 200.0f;

uint32_t beat_frac, prev_beat_frac;
const uint32_t beat_us = 60 * 1000000/ bpm;
uint current_stream = 0;

typedef struct LED_STATE_t_ {
    PIO pio;
    uint offset;
    uint sm;
    uint32_t cycle_idx;
    uint8_t pattern_idx;
} LED_STATE_t;

LED_STATE_t led_state = {
    .cycle_idx = 0,
    .pattern_idx = 0,
};

pattern* pattern_table;
uint pattern_count;

void led_init() {
    // todo get free sm
    led_state.pio = pio0;
    led_state.offset = pio_add_program(led_state.pio, &ws2812_program);

    get_all_patterns_table(pattern_table, &pattern_count);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    led_state.sm = pio_claim_unused_sm(led_state.pio, true);

    ws2812_program_init(led_state.pio, led_state.sm, led_state.offset, WS2812_PIN, 800000, IS_RGBW);
    dma_init(led_state.pio, led_state.sm);

    printf("Initialized LED driver\n");
}

void led_update_pattern_idx(uint8_t pattern_idx) {
    if (pattern_idx >= count_of(pattern_table)) 
    {
        puts("Index out of range, skipping...");
        return;
    }
    led_state.pattern_idx = pattern_idx;
    printf("Updated pattern to: %s\n", pattern_table[led_state.pattern_idx].name);
    // puts(dir == 1 ? "(forward)" : "(backward)");
}

void led_set_random_pattern() {
    led_update_pattern_idx(rand() % pattern_count);
    int dir = (rand() >> 30) & 1 ? 1 : -1;

}

void led_beat() {
   led_state.cycle_idx = 0; 
}

void led_update() {
    absolute_time_t current_time = get_absolute_time();	
    int64_t duration_from_time_ref =	absolute_time_diff_us(time_ref, current_time);
    beat_frac = (duration_from_time_ref % beat_us) * UINT32_MAX / beat_us;
    if (beat_frac < prev_beat_frac) {
        puts("Beat ... ");
    }

    run_pattern(led_state.pattern_idx, colors[current_stream], NUM_PIXELS, beat_frac);

    output_strings_dma(colors[current_stream]);
    current_stream ^= 1;
    prev_beat_frac = beat_frac;

    led_state.cycle_idx++;
}

