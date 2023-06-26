#include "fade.h"

#include <stddef.h>
#include <stdint.h>

void pattern_fade_init();

void pattern_fade_new(uint32_t *stream, size_t len, uint8_t t) {
  uint8_t value = 127 - (t >> 1);
  uint32_t v = value * 0x01010100;

  for (int i = 0; i < len; ++i) {
    stream[i] = v;
  }
}