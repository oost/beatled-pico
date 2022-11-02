#include "ws2812_patterns.h"
#include "ws2812_config.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t gamma8[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,
    2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,
    4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,
    8,   9,   9,   9,   10,  10,  10,  11,  11,  11,  12,  12,  13,  13,  13,
    14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,  20,  21,
    21,  22,  22,  23,  24,  24,  25,  25,  26,  27,  27,  28,  29,  29,  30,
    31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,  41,  42,
    43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  52,  54,  55,  56,  57,
    58,  59,  60,  61,  62,  63,  64,  66,  67,  68,  69,  70,  72,  73,  74,
    75,  77,  78,  79,  81,  82,  83,  85,  86,  87,  89,  90,  92,  93,  95,
    96,  98,  99,  101, 102, 104, 105, 107, 109, 110, 112, 114, 115, 117, 119,
    120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142, 144, 146,
    148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175, 177,
    180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252,
    255};

static inline uint32_t rgb_u32(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)(r) << 16) | ((uint32_t)(g) << 24) | (uint32_t)(b) << 8;
}

void pattern_snakes(uint32_t *stream, size_t len, uint32_t t) {
  uint32_t pos = t >> 26;

  for (uint i = 0; i < len; ++i) {
    uint x = (i + (pos >> 1)) % 64;
    uint32_t value;
    if (x < 10)
      value = (rgb_u32(0xff, 0, 0));
    else if (x >= 15 && x < 25)
      value = (rgb_u32(0, 0xff, 0));
    else if (x >= 30 && x < 40)
      value = (rgb_u32(0, 0, 0xff));
    else
      value = (0);
    stream[i] = value;
  }
}

void pattern_random(uint32_t *stream, size_t len, uint32_t t) {
  if ((t >> 29) == 0)
    return;
  for (int i = 0; i < len; ++i)
    stream[i] = (rand());
}

void pattern_sparkle(uint32_t *stream, size_t len, uint32_t t) {
  if ((t >> 29) == 0)
    return;
  for (int i = 0; i < len; ++i)
    stream[i] = (rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(uint32_t *stream, size_t len, uint32_t t) {
  int max = 100; // let's not draw too much current!
  uint8_t brightness = t >> 26;
  for (int i = 0; i < len; ++i) {
    stream[i] = (brightness * 0x10101);
  }
}

void pattern_drops(uint32_t *stream, size_t len, uint32_t t) {
  int value;
  int max = (UINT32_MAX / 256);
  int pos = t % NUM_PIXELS;
  int step = max / NUM_PIXELS * 3;
  for (int i = 0; i < len; ++i) {
    if (i > pos) {
      value = 0;
    } else {
      value = (max - (pos - i) * step);
    }
    if (value < 0) {
      value = 0;
    }

    value = gamma8[value];
    stream[i] = rgb_u32(value, value, value);
  }
}

void pattern_solid(uint32_t *stream, size_t len, uint32_t t) {
  uint8_t pos = t >> 24;
  for (int i = 0; i < len; ++i) {
    stream[i] = (pos * 0x10101);
  }
}

int level = 8;

void pattern_fade(uint32_t *stream, size_t len, uint32_t t) {
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
    stream[i] = slow_t;
  }
}

const pattern _pattern_table[] = {
    {pattern_snakes, "Snakes!"},   {pattern_random, "Random data"},
    {pattern_sparkle, "Sparkles"}, {pattern_greys, "Greys"},
    {pattern_drops, "Drops"},      {pattern_solid, "Solid!"},
    {pattern_fade, "Fade"},
};

size_t num_patterns() {
  return (sizeof(_pattern_table) / sizeof((_pattern_table)[0]));
}

void get_all_patterns_table(const pattern *pattern_table,
                            size_t *pattern_count) {
  pattern_table = _pattern_table;
  *pattern_count = num_patterns();
}

void run_pattern(int pattern_idx, uint32_t *stream, size_t len,
                 uint32_t beat_pos) {
  pattern_idx = pattern_idx % num_patterns();
  _pattern_table[pattern_idx].pat(stream, len, beat_pos);
}
