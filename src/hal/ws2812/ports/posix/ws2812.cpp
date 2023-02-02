#include <cstdbool>
#include <cstdint>
#include <memory>

#include "hal/ws2812.h"

class DMAQueue {
public:
  DMAQueue(uint16_t num_pixel, uint32_t frequency, bool is_rgbw)
      : num_pixel_{num_pixel}, frequency_{frequency}, is_rgbw_{is_rgbw} {}

  void update_buffer(uint32_t *stream) {}

private:
  uint16_t num_pixel_;
  uint32_t frequency_;
  bool is_rgbw_;
};

std::unique_ptr<DMAQueue> dma_queue;

void ws2812_init(uint16_t num_pixel, uint8_t ws2812_pin, uint32_t frequency,
                 bool is_rgbw) {
  dma_queue = std::make_unique<DMAQueue>(num_pixel, frequency, is_rgbw);
}

void output_strings_dma(uint32_t *stream) {}
