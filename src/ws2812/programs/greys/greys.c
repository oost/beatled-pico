#include "greys.h"

void pattern_greys(uint32_t *stream, size_t len, uint8_t t,
                   uint32_t beat_count) {
  int max = 100; // let's not draw too much current!
  uint8_t brightness = t >> 2;
  for (int i = 0; i < len; ++i) {
    stream[i] = (brightness * 0x10101);
  }
}