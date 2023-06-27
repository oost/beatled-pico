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
  uint8_t intensity = 127 - (t >> 1);
  uint8_t pos = t >> 2;

  uint32_t value;
  for (unsigned int i = 0; i < len; ++i) {
    unsigned int x = (i + (pos >> 1)) % 64;
    if (x < 10)
      value = (rgb_u32(intensity, 0, 0));
    else if (x >= 15 && x < 25)
      value = (rgb_u32(0, intensity, 0));
    else if (x >= 30 && x < 40)
      value = (rgb_u32(0, 0, intensity));
    else
      value = (0);
    stream[i] = value;
  }
}
