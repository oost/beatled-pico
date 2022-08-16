#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "command_queue/queue.h"
#include "pico/util/queue.h"

#define MAX_QUEUE_COUNT 20
queue_t command_queue;

void command_queue_init()
{
  queue_init(&command_queue, sizeof(envelope_t), MAX_QUEUE_COUNT);
}

int command_queue_add_message(envelope_t *envelope) 
{
  // Not sure if this is necessary with queue_try_add...
  if (queue_is_full(&command_queue)) {
    puts("Queued is FULL!!!");
    return 1;
  }

  if (!queue_try_add(&command_queue, envelope)) {
    puts("Couldn't add... :-(");
    return 1;
  }
  puts("Queued message");
}

int command_queue_pop_message(envelope_t *envelope) 
{
  // Not sure if this is necessary with queue_try_removes...
  if (queue_is_empty(&command_queue)) {
    return 1;
  }
  if (!queue_try_remove(&command_queue, envelope)) {
    return 1;
  }

  puts("Popped a message from queue");
  return 0;
}