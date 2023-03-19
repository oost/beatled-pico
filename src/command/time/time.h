#ifndef COMMAND__TIME__TIME_H
#define COMMAND__TIME__TIME_H

#include <stddef.h>

#include "beatled/protocol.h"

int send_time_request();

int validate_time_msg(beatled_message_t *server_msg, size_t data_length,
                      uint64_t dest_time);

int process_time_msg(beatled_message_t *server_msg, size_t data_length,
                     uint64_t dest_time);

#endif // COMMAND__TIME__TIME_H