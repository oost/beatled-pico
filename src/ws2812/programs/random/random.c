#include "random.h"

#include <stdlib.h>

#include "../utils.h"

void pattern_random(uint32_t *stream, size_t len, uint8_t t,
                    uint32_t beat_count) {
  // Freeze pattern at beat start for visual "hit"
  if (t < 8) {
    return;  // Keep previous frame for first 3% of beat
  }

  // Update frequency decreases through beat: fast changes → slow
  uint8_t update_mask;
  if (t < 64) {
    update_mask = 0x03;  // Update every 4 calls (very fast)
  } else if (t < 128) {
    update_mask = 0x07;  // Update every 8 calls (medium)
  } else {
    update_mask = 0x0F;  // Update every 16 calls (slow)
  }

  if ((t & update_mask) != 0) {
    return;  // Skip this frame
  }

  // Random colors with beat-synchronized brightness
  uint8_t max_brightness = beat_intensity_quadratic(t) >> 4;  // 0-15 range

  for (int i = 0; i < len; ++i) {
    uint8_t r = (rand() % max_brightness);
    uint8_t g = (rand() % max_brightness);
    uint8_t b = (rand() % max_brightness);
    stream[i] = rgb_u32(r, g, b);
  }
}