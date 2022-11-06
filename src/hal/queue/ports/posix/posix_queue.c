#include <stdbool.h>
#include <stdlib.h>

#include "circular_buffer.h"
#include "hal/queue.h"

hal_queue_handle_t hal_queue_init(size_t msg_size, int queue_size) {
  queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
  queue_init(new_queue, msg_size, queue_size);
  return new_queue;
}

void hal_queue_free(hal_queue_handle_t queue) { queue_free(queue); }

unsigned int hal_queue_size(hal_queue_handle_t queue) {
  return queue_get_level(queue);
}

unsigned int hal_queue_capacity(hal_queue_handle_t queue) {
  return queue_get_capacity(queue);
}

// unsigned int hal_queue_capacity(hal_queue_handle_t queue) {
//   return circular_buf_capacity(queue);
// }

bool hal_queue_add_message(hal_queue_handle_t queue, void *data) {
  return queue_try_add(queue, data);
}
void hal_queue_add_message_blocking(hal_queue_handle_t queue, void *data) {
  queue_add_blocking(queue, data);
}
bool hal_queue_pop_message(hal_queue_handle_t queue, void *data) {
  return queue_try_remove(queue, data);
}
void hal_queue_pop_message_blocking(hal_queue_handle_t queue, void *data) {
  queue_remove_blocking(queue, data);
}
