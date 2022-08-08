#ifndef WS2812_H
#define WS2812_H


void led_init();

void led_update_pattern_idx(uint8_t pattern_idx);

void led_set_random_pattern();

void led_update();

#endif // WS2812_H