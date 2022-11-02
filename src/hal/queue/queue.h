#ifndef HAL__QUEUE__QUEUE_H
#define HAL__QUEUE__QUEUE_H

void *hal_queue_init(size_t msg_size, int queue_size);

bool hal_pico_queue_add_message(void *queue, void *data);
bool hal_queue_pop_message(void *queue, void *data);

#endif // HAL__QUEUE__QUEUE_H