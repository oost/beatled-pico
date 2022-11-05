#include <stdbool.h>

#include "circular_buffer.h"
#include "hal/queue.h"

hal_queue_handle_t hal_queue_init(size_t msg_size, int queue_size) {
  return circular_buf_init(msg_size, queue_size);
}

void hal_queue_free(hal_queue_handle_t queue) { circular_buf_free(queue); }

bool hal_queue_add_message(hal_queue_handle_t queue, void *data) {
  return false;
}
bool hal_queue_add_message_blocking(hal_queue_handle_t queue, void *data) {
  return false;
}
bool hal_queue_pop_message(hal_queue_handle_t queue, void *data) {
  return false;
}
bool hal_queue_pop_message_blocking(hal_queue_handle_t queue, void *data) {
  return false;
}
