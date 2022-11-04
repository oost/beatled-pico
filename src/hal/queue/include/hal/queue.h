#ifndef HAL__QUEUE__QUEUE_H
#define HAL__QUEUE__QUEUE_H

#include <stddef.h>

/// Opaque circular buffer structure
typedef struct hal_queue_t hal_queue_t;

/// Handle type, the way users interact with the API
typedef hal_queue_t *hal_queue_handle_t;

hal_queue_handle_t hal_queue_init(size_t msg_size, int queue_size);

bool hal_queue_add_message(hal_queue_handle_t queue, void *data);
bool hal_queue_pop_message(hal_queue_handle_t queue, void *data);

#endif // HAL__QUEUE__QUEUE_H