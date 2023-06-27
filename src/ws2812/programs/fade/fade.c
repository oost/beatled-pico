#include "fade.h"

#include <stddef.h>
#include <stdint.h>

#include "../utils.h"

void pattern_fade_init() {}

void pattern_fade_grey(uint32_t *stream, size_t len, uint8_t t,
                       uint32_t beat_count) {
  uint8_t value = 16 - (t >> 4);
  uint32_t v = value * 0x01010100;

  for (int i = 0; i < len; ++i) {
    stream[i] = v;
  }
}

void pattern_fade_color(uint32_t *stream, size_t len, uint8_t t,
                        uint32_t beat_count) {
  uint8_t value = 16 - (t >> 4);

  uint32_t color = convert_hsv_to_rgb((beat_count << 2) % 255, 255, value);

  for (int i = 0; i < len; ++i) {
    stream[i] = color;
  }
}
