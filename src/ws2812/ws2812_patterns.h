#ifndef WS2812_PATTERNS_H
#define WS2812_PATTERNS_H

#include <stddef.h>
#include <stdint.h>

typedef void (*pattern_fn)(uint32_t *stream, size_t len, uint8_t beat_pos);
typedef void (*pattern_init_fn)();

typedef struct {
  pattern_fn pattern_fn;
  pattern_init_fn pattern_init_fn;
  const char *name;
} pattern;

// size_t num_patterns();
size_t get_pattern_count();

const char *pattern_get_name(uint8_t pattern_idx);

void run_pattern(int pattern_idx, uint32_t *stream, size_t len,
                 uint8_t beat_pos);

static inline uint32_t rgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 16) | ((uint32_t)(g) << 24) | (uint32_t)(b) << 8;
}

#endif // WS2812_PATTERNS_H
