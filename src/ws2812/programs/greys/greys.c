#include "greys.h"

#include "../utils.h"

void pattern_greys(uint32_t *stream, size_t len, uint8_t t,
                   uint32_t beat_count) {
  // Use exponential-like curve for dramatic punch
  uint8_t brightness = beat_intensity_exp4(t) >> 3;  // 0-31 range

  // Apply gamma correction for perceptually linear fade
  brightness = gamma8[brightness];

  uint32_t color = rgb_u32(brightness, brightness, brightness);

  for (int i = 0; i < len; ++i) {
    stream[i] = color;
  }
}