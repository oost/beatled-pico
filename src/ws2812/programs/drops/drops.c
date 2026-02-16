#include "drops.h"
#include "../../ws2812_config.h"
#include "../utils.h"

void pattern_drops(uint32_t *stream, size_t len, uint8_t t,
                   uint32_t beat_count) {
  // Drop position sweeps across all LEDs within one beat
  int pos = (t * NUM_PIXELS) >> 8;  // Map 0-255 to 0-NUM_PIXELS

  // Intensity decays with beat for overall "fade out" effect
  uint8_t max_intensity = beat_intensity_quadratic(t) >> 2;  // 0-63 range

  for (int i = 0; i < len; ++i) {
    int value;
    if (i > pos) {
      value = 0;  // Drop hasn't reached this LED yet
    } else {
      // Gradient trail: brighter near drop position, fades behind it
      int distance = pos - i;
      int trail_length = NUM_PIXELS / 3;  // Trail covers 1/3 of strip

      if (distance < trail_length) {
        value = max_intensity - (distance * max_intensity / trail_length);
      } else {
        value = 0;
      }
    }

    // Apply gamma correction and convert to RGB
    value = gamma8[value];

    // Color cycles through hue spectrum every 16 beats
    uint8_t hue = (beat_count << 4) % 256;
    stream[i] = convert_hsv_to_rgb(hue, 255, value);
  }
}