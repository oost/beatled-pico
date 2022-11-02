#ifndef WS2812_PATTERNS_H
#define WS2812_PATTERNS_H

#include <stddef.h>
#include <stdint.h>

typedef void (*pattern_fn)(uint32_t *stream, size_t len, uint32_t beat_pos);

typedef struct {
  pattern_fn pat;
  const char *name;
} pattern;

void get_all_patterns_table(const pattern *pattern_table,
                            size_t *pattern_count);

size_t num_patterns();

void run_pattern(int pattern_idx, uint32_t *stream, size_t len,
                 uint32_t beat_pos);

#endif // WS2812_PATTERNS_H
