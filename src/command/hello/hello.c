#include "hal/unique_id.h"

#include "beatled/protocol.h"
#include "command/utils.h"
#include "constants.h"
#include "hal/blink.h"
#include "hal/udp.h"
#include "hello.h"
#include "state_manager/state_manager.h"

int prepare_hello_request(void *buffer_payload, size_t buf_len) {
  if (buf_len != sizeof(beatled_hello_msg_t)) {
    printf("Error sizes don't match %zu, %zu", buf_len,
           sizeof(beatled_hello_msg_t));
    return 1;
  }

  beatled_hello_msg_t *msg = (beatled_hello_msg_t *)buffer_payload;
  msg->base.type = eBeatledHello;
  // pico_get_unique_board_id_string(msg->board_id, count_of(msg->board_id));
  return 0;
}

int send_hello_request() {
  puts("Sending hello request");
  return send_udp_request(sizeof(beatled_hello_msg_t), prepare_hello_request);
}

int process_hello_msg(beatled_message_t *server_msg, size_t data_length) {
  puts("Hello!");
  if (!check_size(data_length, sizeof(beatled_hello_response_t))) {
    return 1;
  }

  beatled_hello_response_t *hello_msg = (beatled_hello_response_t *)server_msg;
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);
  state_manager_set_state(STATE_REGISTERED);

  return 0;
}
