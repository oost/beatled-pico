#include <stdio.h>
#include <stdlib.h>

#include "beatled/protocol.h"
#include "command/command.h"
#include "command/utils.h"
#include "constants.h"
#include "hal/blink.h"
#include "tempo/tempo.h"
#include "time/time.h"

int command_program(beatled_message_t *server_msg, size_t data_length) {
  puts("Program!");
  if (!check_size(data_length, sizeof(beatled_program_msg_t))) {
    return 1;
  }
  beatled_program_msg_t *program_msg = (beatled_program_msg_t *)server_msg;

  // led_update_pattern_idx(program_msg->program_id);
  return 0;
}

int command_error(beatled_message_t *server_msg, size_t data_length) {
  puts("Error");
  if (!check_size(data_length, sizeof(beatled_error_msg_t))) {
    return 1;
  }
  beatled_error_msg_t *error_msg = (beatled_error_msg_t *)server_msg;

  printf("Communication error %u\n", error_msg->error_code);
  return 0;
}

int handle_server_message(void *event_data, size_t data_length,
                          uint64_t dest_time) {
  if (sizeof(beatled_message_t) >= data_length) {
    return 1;
  }
  beatled_message_t *server_msg = (beatled_message_t *)event_data;

  int err = 0;
  switch (server_msg->type) {
  case eBeatledHello:
    err = process_hello_msg(server_msg, data_length);
    break;

  case eBeatledProgram:
    err = command_program(server_msg, data_length);
    break;

  case eBeatledTempo:
    err = process_tempo_msg(server_msg, data_length);
    break;

  case eBeatledTime:
    err = process_time_msg(server_msg, data_length, dest_time);
    break;

  case eBeatledError:
    err = command_error(server_msg, data_length);
    break;

  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }

  return err;
}

int handle_event(event_t *event) {
  puts("Handling event");

  int err;
  switch (event->event_type) {
  case event_server_message:
    err = handle_server_message(event->data, event->data_length, event->time);
    break;

  case event_error:
    puts("Error event command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
    break;

  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }
  if (event->data != NULL) {
    free(event->data);
  }
  return err;
}
