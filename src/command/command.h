#ifndef COMMAND__COMMAND_H
#define COMMAND__COMMAND_H

#include <stdlib.h>

#include "command/hello/hello.h"
#include "command/tempo/tempo.h"
#include "command/time/time.h"
#include "event_queue/queue.h"

int handle_event(event_t *event);

#endif // COMMAND__COMMAND_H