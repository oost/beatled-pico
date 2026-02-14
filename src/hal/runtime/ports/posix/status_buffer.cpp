#include <atomic>

#include "status_buffer.h"

static StatusBuffer::Ptr instance_ = std::make_shared<StatusBuffer>();

StatusBuffer::Ptr StatusBuffer::load_instance() {
  std::atomic_thread_fence(std::memory_order_relaxed);
  return instance_;
}

void StatusBuffer::update(const StatusData &d) {
  auto new_instance = std::make_shared<StatusBuffer>(d);
  std::atomic_thread_fence(std::memory_order_release);
  instance_ = new_instance;
}
