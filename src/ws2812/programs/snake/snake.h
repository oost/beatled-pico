#ifndef WS2812__PROGRAMS__SNAKE__SNAKE_H
#define WS2812__PROGRAMS__SNAKE__SNAKE_H

#include "../../ws2812_patterns.h"

void pattern_snakes_init();
void pattern_snakes_new(uint32_t *stream, size_t len, uint8_t t);

#endif // WS2812__PROGRAMS__SNAKE__SNAKE_H