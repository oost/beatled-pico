#ifndef COMMAND__COMMAND_H
#define COMMAND__COMMAND_H

#include <pico/stdlib.h>

#include "command_queue/queue.h"

int parse_command(command_envelope_t *envelope);

#endif // COMMAND__COMMAND_H