#include <stdio.h>
#include <stdlib.h>

#include "beatled/protocol.h"
#include "command/command.h"
#include "command/next_beat.h"
#include "command/tempo.h"
#include "command/time.h"
#include "command/utils.h"
#include "config/constants.h"
#include "hal/blink.h"
#include "hal/network.h"
#include "hal/registry.h"
#include "process/intercore_queue.h"
#include "state_manager/state_manager.h"

int command_program(beatled_message_t *server_msg, size_t data_length) {
  puts("Program!");
  if (!check_size(data_length, sizeof(beatled_message_program_t))) {
    return 1;
  }
  beatled_message_program_t *program_msg =
      (beatled_message_program_t *)server_msg;

  uint16_t program_id = ntohs(program_msg->program_id);
  printf("Setting program to %u\n", program_id);

  registry_lock_mutex();
  registry.program_id = program_id;
  registry_unlock_mutex();

  intercore_message_t msg = {.message_type = 0x01 << intercore_program_update};
  if (!hal_queue_add_message(intercore_command_queue, &msg)) {
    puts("Intercore queue is FULL!!!");
    return 1;
  }

  return 0;
}

int command_error(beatled_message_t *server_msg, size_t data_length) {
  puts("Error");
  if (!check_size(data_length, sizeof(beatled_message_error_t))) {
    return 1;
  }
  beatled_message_error_t *error_msg = (beatled_message_error_t *)server_msg;

  printf("Communication error %u\n", error_msg->error_code);
  return 0;
}

int validate_server_message(void *event_data, size_t data_length) {
  if (!event_data || sizeof(beatled_message_t) >= data_length) {
    return 1;
  }
  beatled_message_t *server_msg = (beatled_message_t *)event_data;

  int err = 0;
  switch (server_msg->type) {
  case BEATLED_MESSAGE_HELLO_RESPONSE:
    err = !(sizeof(beatled_message_hello_response_t) == data_length);
    break;

  case BEATLED_MESSAGE_PROGRAM:
    err = !(sizeof(beatled_message_program_t) == data_length);
    break;

  case BEATLED_MESSAGE_TEMPO_RESPONSE:
    err = !(sizeof(beatled_message_tempo_response_t) == data_length);
    break;

  case BEATLED_MESSAGE_TIME_RESPONSE:
    err = !(sizeof(beatled_message_time_response_t) == data_length);
    break;

  case BEATLED_MESSAGE_NEXT_BEAT:
    err = !(sizeof(beatled_message_next_beat_t) == data_length);
    break;

  case BEATLED_MESSAGE_ERROR:
    err = !(sizeof(beatled_message_error_t) == data_length);
    break;

  default:
    printf("Unknown command type %d...\n", server_msg->type);
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }

  return err;
}

int handle_server_message(void *event_data, size_t data_length,
                          uint64_t dest_time) {
  if (!event_data || sizeof(beatled_message_t) >= data_length) {
    return 1;
  }
  beatled_message_t *server_msg = (beatled_message_t *)event_data;

  int err = 0;
  switch (server_msg->type) {
  case BEATLED_MESSAGE_HELLO_RESPONSE:
    err = process_hello_msg(server_msg, data_length);
    break;

  case BEATLED_MESSAGE_PROGRAM:
    err = command_program(server_msg, data_length);
    break;

  case BEATLED_MESSAGE_TEMPO_RESPONSE:
    err = process_tempo_msg(server_msg, data_length);
    break;

  case BEATLED_MESSAGE_TIME_RESPONSE:
    err = process_time_msg(server_msg, data_length, dest_time);
    break;

  case BEATLED_MESSAGE_NEXT_BEAT:
    err = process_next_beat_msg(server_msg, data_length);
    break;

  case BEATLED_MESSAGE_ERROR:
    err = command_error(server_msg, data_length);
    break;

  default:
    printf("Unknown message type %d...\n", server_msg->type);
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }

  return err;
}

int handle_state_change(void *event_data) {
  state_event_t *state_event = (state_event_t *)event_data;
  state_manager_set_state(state_event->next_state);
  return 0;
}

int handle_event(event_t *event) {
  // puts("Handling event");
  int err;
  switch (event->event_type) {
  case event_server_message:
    // err = validate_server_message(event->data, event->data_length);
    err = handle_server_message(event->data, event->data_length, event->time);
    break;

  case event_state_transition:
    err = handle_state_change(event->data);
    break;

  case event_error:
    puts("Error event command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
    break;

  default:
    printf("Unknown event type %d...\n", event->event_type);
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }
  // puts("Event handled.");
  if (event->data != NULL) {
    // puts("Deleteing event data.");
    // printf("Freeing memory at %x\n", event->data);
    free(event->data);
  }

  return err;
}
