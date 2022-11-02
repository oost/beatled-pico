#ifndef HAL__WS2812__WS2812_H
#define HAL__WS2812__WS2812_H

void ws2812_init(uint16_t num_pixel, uint8_t ws2812_pin, uint32_t frequency,
                 bool is_rgbw);

void output_strings_dma();

#endif // HAL__WS2812__WS2812_H