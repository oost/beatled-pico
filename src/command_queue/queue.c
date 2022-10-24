#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_queue/queue.h"

#define MAX_QUEUE_COUNT 20

static queue_t command_queue;

void command_queue_init() {
  queue_init(&command_queue, sizeof(command_envelope_t), MAX_QUEUE_COUNT);
}

bool command_envelope_message_alloc(command_envelope_t *envelope,
                                    size_t message_length) {
  envelope->message = (char *)malloc(message_length);
  envelope->message_length = message_length;
  return envelope->message != NULL;
}

void command_envelope_message_free(command_envelope_t *envelope) {
  free(envelope->message);
}

bool command_queue_add_message(command_envelope_t *envelope) {
  if (!queue_try_add(&command_queue, envelope)) {
    puts("Queued is FULL!!!");
    return false;
  }
  puts("Queued message");
  return true;
}

bool command_queue_pop_message(command_envelope_t *envelope) {
  if (!queue_try_remove(&command_queue, envelope)) {
    return false;
  }

  puts("Popped a message from queue");
  return true;
}