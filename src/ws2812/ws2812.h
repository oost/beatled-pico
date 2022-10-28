#ifndef WS2812_H
#define WS2812_H

void led_init();

void led_update_pattern_idx(uint8_t pattern_idx);

void led_set_random_pattern();

void led_update();

void led_update_tempo(uint32_t new_tempo_period_us);

void led_update_time_ref(uint64_t new_time_ref);

void led_beat();

#endif // WS2812_H