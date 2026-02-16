#include "snake.h"

#include "../utils.h"

static uint8_t position_;
static uint8_t speed_;

void pattern_snakes_init() {
  position_ = 0;
  speed_ = 1;
}

void pattern_snakes(uint32_t *stream, size_t len, uint8_t t,
                    uint32_t beat_count) {
  // Use quadratic curve for more pronounced beat response
  uint8_t base_intensity = beat_intensity_quadratic(t) >> 1;  // 0-127 range
  uint8_t pos = t >> 2;  // Position moves 0-64 within beat

  // Rotate colors every 4 beats for variety
  uint8_t color_offset = (beat_count & 0x3) * 64;  // 0, 64, 128, 192

  for (unsigned int i = 0; i < len; ++i) {
    unsigned int x = (i + (pos >> 1)) % 64;
    uint8_t intensity = base_intensity;

    uint32_t value;
    if (x < 10)
      value = rgb_u32(intensity + color_offset, 0, 0);
    else if (x >= 15 && x < 25)
      value = rgb_u32(0, intensity + color_offset, 0);
    else if (x >= 30 && x < 40)
      value = rgb_u32(0, 0, intensity + color_offset);
    else
      value = 0;
    stream[i] = value;
  }
}
