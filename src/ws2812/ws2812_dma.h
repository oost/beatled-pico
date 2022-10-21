#ifndef WS2812_DMA_H
#define WS2812_DMA_H

#include "hardware/pio.h"

void dma_init(PIO pio, uint sm);

void output_strings_dma(uint32_t *stream);


#endif // WS2812_DMA_H
