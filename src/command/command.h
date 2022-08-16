#ifndef COMMAND_COMMAND_H
#define COMMAND_COMMAND_H

#include "pico/stdlib.h"

int parse_command(char * command, uint16_t message_length);

#endif // COMMAND_COMMAND_H