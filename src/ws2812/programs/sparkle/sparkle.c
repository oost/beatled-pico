#include "sparkle.h"

#include "../utils.h"

void pattern_sparkle(uint32_t *stream, size_t len, uint8_t t,
                     uint32_t beat_count) {
  // Sparkle density decreases through beat: high at start, sparse at end
  uint8_t density_threshold;
  if (t < 32) {
    density_threshold = 4;  // ~25% sparkle density at beat start
  } else if (t < 128) {
    density_threshold = 8;  // ~12% density in first half
  } else {
    density_threshold = 16; // ~6% density in second half
  }

  // Brightness also fades with beat
  uint8_t brightness = beat_intensity_quadratic(t) >> 3;  // 0-31 range
  uint32_t color = rgb_u32(brightness, brightness, brightness);

  for (int i = 0; i < len; ++i) {
    stream[i] = (rand() % density_threshold == 0) ? color : 0;
  }
}
