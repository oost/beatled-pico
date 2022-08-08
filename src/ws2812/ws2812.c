/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 30

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 2
#endif

const uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
    // pio_sm_put_blocking(pio0, 0, pixel_grb);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

void pattern_snakes(uint len, uint t) {
    for (uint i = 0; i < len; ++i) {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(urgb_u32(0, 0, 0xff));
        else
            put_pixel(0);
    }
}

void pattern_random(uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand());
}

void pattern_sparkle(uint len, uint t) {
    if (t % 8)
        return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(uint len, uint t) {
    int max = 100; // let's not draw too much current!
    t %= max;
    for (int i = 0; i < len; ++i) {
        put_pixel(t * 0x10101);
        if (++t >= max) t = 0;
    }
}

void pattern_drops(uint len, uint t) {
    int value;
    int max = 50;
    int pos = t % NUM_PIXELS;
    int step = max / NUM_PIXELS * 3;
    for( int i = 0; i < len; ++i) {
        if (i > pos) {
            value = 0;
        } else {
            value = (max - (pos-i)*step);
        }
        if (value < 0) { value = 0; }
    
        value = gamma8[value];
        put_pixel(urgb_u32(value, value, value));
    }
}


void pattern_solid(uint len, uint t) {
    t = t % 200;
    for (int i = 0; i < len; ++i) {
        put_pixel(t * 0x10101);
    }
}

int level = 8;

void pattern_fade(uint len, uint t) {
    uint shift = 4;

    uint max = 16; // let's not draw too much current!
    max <<= shift;

    uint slow_t = t / 32;
    slow_t = level;
    slow_t %= max;

    static int error;
    slow_t += error;
    error = slow_t & ((1u << shift) - 1);
    slow_t >>= shift;
    slow_t *= 0x010101;

    for (int i = 0; i < len; ++i) {
        put_pixel(slow_t);
    }
}

typedef void (*pattern)(uint len, uint t);
const struct {
    pattern pat;
    const char *name;
} pattern_table[] = {
        {pattern_snakes,  "Snakes!"},
        {pattern_random,  "Random data"},
        {pattern_sparkle, "Sparkles"},
        {pattern_greys,   "Greys"},
        {pattern_drops,   "Drops"},
        {pattern_solid,  "Solid!"},
       {pattern_fade, "Fade"},
};

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

void led_init() {
    // todo get free sm
    led_state.pio = pio0;
    led_state.offset = pio_add_program(led_state.pio, &ws2812_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    led_state.sm = 0; //pio_claim_unused_sm(led_state.pio, true);

    ws2812_program_init(led_state.pio, led_state.sm, led_state.offset, WS2812_PIN, 800000, IS_RGBW);
    printf("Initialized LED driver\n");
}

void led_update_pattern_idx(uint8_t pattern_idx) {
    led_state.pattern_idx = pattern_idx;
    printf("Updated pattern to: %s\n", pattern_table[led_state.pattern_idx].name);
    // puts(dir == 1 ? "(forward)" : "(backward)");
}

void led_set_random_pattern() {
    led_update_pattern_idx(rand() % count_of(pattern_table));
    int dir = (rand() >> 30) & 1 ? 1 : -1;

}

void led_update() {
    printf("Updating LED\n");

    led_state.cycle_idx++;
    pattern_table[led_state.pattern_idx].pat(NUM_PIXELS, led_state.cycle_idx);
}