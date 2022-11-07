#ifndef COMMAND__COMMAND_H
#define COMMAND__COMMAND_H

#include <stdlib.h>

#include "event/event_queue.h"
#include "hello/hello.h"
#include "tempo/tempo.h"
#include "time/time.h"

int handle_event(event_t *event);

#endif // COMMAND__COMMAND_H