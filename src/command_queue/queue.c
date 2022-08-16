#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "command_queue/queue.h"
#include "pico/util/queue.h"

#define MAX_QUEUE_COUNT 20
queue_t *command_queue;

void command_queue_init()
{
  queue_init(command_queue, sizeof(struct envelope), MAX_QUEUE_COUNT);
}

int command_queue_add_message(char *message, uint16_t message_length, uint16_t port) 
{
  struct envelope * env = malloc(sizeof(struct envelope));
  env->time_received = get_absolute_time();
  env->message_length = message_length;
  env->message = message;
  env->port = port;

  if (queue_is_full(command_queue)) {
    puts("Queued is FULL!!!");
    return 1;
  }

  if (!queue_try_add(command_queue, env)) {
    puts("Couldn't add... :-(");
    return 1;
  }
  puts("Queued message");
}

int command_queue_pop_message(char *message, uint16_t* message_length) 
{
  if (queue_is_empty(command_queue)) {
    return 1;
  }

  struct envelope* env_to_drop = NULL;
  if (!queue_try_remove(command_queue, env_to_drop)) {
    return 1;
  }

  message = env_to_drop->message;
  *message_length = env_to_drop->message_length;
 
  // Free up the memory 
  free(env_to_drop);
  puts("Popped a message from queue");
  return 0;
}