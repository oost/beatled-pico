#include "drops.h"
#include "../../ws2812_config.h"
#include "../utils.h"

void pattern_drops(uint32_t *stream, size_t len, uint8_t t,
                   uint32_t beat_count) {
  int value;
  int max = (UINT8_MAX / 256);
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