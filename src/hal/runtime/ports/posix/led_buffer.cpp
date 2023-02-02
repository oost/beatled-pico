#include <atomic>

#include "led_buffer.h"

LEDBuffer::Ptr instance_ =
    std::make_shared<LEDBuffer>(LEDColor{.red = 0, .blue = 1, .green = 0}, 16);

LEDBuffer::Ptr LEDBuffer::load_instance() {
  std::atomic_thread_fence(std::memory_order_relaxed);
  return instance_;
}

void LEDBuffer::create_instance(uint32_t *stream, uint16_t num_pixel) {
  std::atomic_thread_fence(std::memory_order_release);
  instance_ = std::make_shared<LEDBuffer>(stream, num_pixel);
}