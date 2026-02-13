#include "hal/unique_id.h"

#include "beatled/protocol.h"
#include "command/hello.h"
#include "command/utils.h"
#include "config/constants.h"
#include "hal/blink.h"
#include "hal/network.h"
#include "hal/udp.h"
#include "state_manager/state_manager.h"

int prepare_hello_request(void *buffer_payload, size_t buf_len) {
  if (buf_len != sizeof(beatled_message_hello_request_t)) {
    printf("Error sizes don't match %zu, %zu", buf_len,
           sizeof(beatled_message_hello_request_t));
    return 1;
  }

  beatled_message_hello_request_t *msg =
      (beatled_message_hello_request_t *)buffer_payload;
  msg->base.type = BEATLED_MESSAGE_HELLO_REQUEST;

  get_unique_board_id(msg->board_id);

  // pico_get_unique_board_id_string(msg->board_id,
  // count_of(msg->board_id));
  return 0;
}

int send_hello_request() {
  puts("Sending hello request");
  return send_udp_request(sizeof(beatled_message_hello_request_t),
                          prepare_hello_request);
}

int process_hello_msg(beatled_message_t *server_msg, size_t data_length) {
  puts("Hello!");
  if (!check_size(data_length, sizeof(beatled_message_hello_response_t))) {
    return 1;
  }

  beatled_message_hello_response_t *hello_msg =
      (beatled_message_hello_response_t *)server_msg;
  uint16_t client_id = ntohs(hello_msg->client_id);
  printf("Registered with client id: %d\n", client_id);
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);

  if (!schedule_state_transition(STATE_REGISTERED)) {
    puts("- Can't schedule transition to registered state.");
    return 1;
  }

  return 0;
}
