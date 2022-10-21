#ifndef WS2812_PATTERNS_H
#define WS2812_PATTERNS_H

#include "pico/stdlib.h"

typedef void (*pattern_fn)(uint32_t *stream, uint len, uint32_t beat_pos);

typedef struct {
    pattern_fn pat;
    const char *name;
} pattern;

void get_all_patterns_table(const pattern* pattern_table, uint* pattern_count);

uint num_patterns();

void run_pattern(int pattern_idx, uint32_t *stream, uint len, uint32_t beat_pos);

#endif // WS2812_PATTERNS_H
