#ifndef MANAGER_H
#define MANAGER_H

#include <pico/stdlib.h>

#include "command/constants.h"

typedef struct command_envelope {
  absolute_time_t time_received;
  char *message;
  uint16_t message_length;
} command_envelope_t;

void command_queue_init();

bool command_envelope_message_alloc(command_envelope_t *envelope,
                                    size_t message_length);
void command_envelope_message_free(command_envelope_t *envelope);

bool command_queue_add_message(command_envelope_t *envelope);
bool command_queue_pop_message(command_envelope_t *envelope);

#endif // MANAGER_H