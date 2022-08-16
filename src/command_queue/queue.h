#ifndef MANAGER_H
#define MANAGER_H

#include "pico/stdlib.h"
#include "command/constants.h"

typedef struct envelope {
  absolute_time_t time_received;	
  char message[COMMAND_MAX_LEN];
  uint16_t message_length;
} envelope_t;

void command_queue_init();

int command_queue_add_message(envelope_t *envelope);

int command_queue_pop_message(envelope_t *envelope);

#endif // MANAGER_H