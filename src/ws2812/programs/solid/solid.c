#include "solid.h"

void pattern_solid(uint32_t *stream, size_t len, uint8_t t,
                   uint32_t beat_count) {
  // Use beat position t (0-255) as brightness value
  // Creates a synchronized fade across all LEDs
  uint8_t pos = t;

  // Pack brightness into all RGB channels for white/grey color
  // Multiplying by 0x10101 sets R=pos, G=pos, B=pos
  // This creates a simple linear fade synchronized to beat position
  for (int i = 0; i < len; ++i) {
    stream[i] = (pos * 0x10101);
  }
}
