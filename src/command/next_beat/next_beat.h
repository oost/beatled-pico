#ifndef SRC__COMMAND__NEXT_BEAT__NEXT_BEAT__H_
#define SRC__COMMAND__NEXT_BEAT__NEXT_BEAT__H_

#include "beatled/protocol.h"

int process_next_beat_msg(beatled_message_t *server_msg, size_t data_length);

#endif // SRC__COMMAND__NEXT_BEAT__NEXT_BEAT__H_