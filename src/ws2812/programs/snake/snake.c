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
  // Calculate base intensity using quadratic decay for musical beat response
  // Peaks at 255 when t=0 (beat start), decays to 0 when t=255 (beat end)
  uint8_t base_intensity = beat_intensity_quadratic(t) >> 1;  // Right-shift by 1 divides by 2, giving 0-127 range

  // Calculate position for smooth snake movement within each beat
  // Dividing t by 4 slows down movement: 0-255 becomes 0-63
  uint8_t pos = t >> 2;  // Right-shift by 2 divides by 4

  // Rotate through 4 different color shifts every 4 beats for visual variety
  // beat_count & 0x3 extracts last 2 bits: cycles 0, 1, 2, 3, 0, 1, 2, 3...
  // Multiply by 64 to get offset values: 0, 64, 128, 192
  uint8_t color_offset = (beat_count & 0x3) * 64;  // Changes every 4 beats

  // Iterate through each LED position
  for (unsigned int i = 0; i < len; ++i) {
    // Calculate virtual position that wraps around at 64
    // (pos >> 1) moves snake slowly, adding i makes it a moving pattern
    // Modulo 64 creates wraparound for seamless animation
    unsigned int x = (i + (pos >> 1)) % 64;

    // Use the calculated base intensity for this beat position
    uint8_t intensity = base_intensity;

    // Create three colored "snake" bands at different positions
    uint32_t value;
    if (x < 10)  // First band: positions 0-9 = red snake
      value = rgb_u32(intensity + color_offset, 0, 0);
    else if (x >= 15 && x < 25)  // Second band: positions 15-24 = green snake
      value = rgb_u32(0, intensity + color_offset, 0);
    else if (x >= 30 && x < 40)  // Third band: positions 30-39 = blue snake
      value = rgb_u32(0, 0, intensity + color_offset);
    else  // All other positions are dark (gaps between snakes)
      value = 0;

    // Write calculated color to LED position
    stream[i] = value;
  }
}
