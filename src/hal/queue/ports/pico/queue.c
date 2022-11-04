// #include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>
#include <stdlib.h>

#include "hal/queue.h"

hal_queue_handle_t hal_queue_init(size_t msg_size, int queue_size) {
  queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
  queue_init(new_queue, msg_size, queue_size);
  return new_queue;
}

bool hal_queue_add_message(hal_queue_handle_t queue, void *data) {
  if (!queue_try_add(queue, data)) {
    puts("Queued is FULL!!!");
    return false;
  }
  puts("Queued message");
  return true;
}

bool hal_queue_pop_message(hal_queue_handle_t queue, void *data) {
  if (!queue_try_remove(queue, data)) {
    return false;
  }

  puts("Popped a message from queue");
  return true;
}