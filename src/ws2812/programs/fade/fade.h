#ifndef SRC__WS2812__PROGRAMS__FADE__FADE__H_
#define SRC__WS2812__PROGRAMS__FADE__FADE__H_

#include "../../ws2812_patterns.h"

void pattern_fade_init();
void pattern_fade_new(uint32_t *stream, size_t len, uint8_t t);

#endif // SRC__WS2812__PROGRAMS__FADE__FADE__H_