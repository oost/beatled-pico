#include "sparkle.h"

void pattern_sparkle(uint32_t *stream, size_t len, uint8_t t,
                     uint32_t beat_count) {
  if ((t >> 5) == 0)
    return;
  for (int i = 0; i < len; ++i)
    stream[i] = (rand() % 16 ? 0 : 0x20202020);
}
