#include <pico/unique_id.h>

#include "beatled/protocol.h"
#include "blink/blink.h"
#include "command/utils.h"
#include "hello.h"
#include "state_manager/state_manager.h"
#include "udp_server/udp_server.h"

int prepare_hello_request(struct pbuf *buffer, size_t buf_len) {
  if (buf_len != sizeof(beatled_hello_msg_t)) {
    printf("Error sizes don't match %d, %d", buf_len,
           sizeof(beatled_hello_msg_t));
    return 1;
  }

  beatled_hello_msg_t *msg = (beatled_hello_msg_t *)buffer->payload;
  msg->base.type = eBeatledHello;
  pico_get_unique_board_id_string(msg->board_id, count_of(msg->board_id));
  return 0;
}

int send_hello_request() {
  return send_udp_request(sizeof(beatled_hello_msg_t), prepare_hello_request);
}

int process_hello_msg(beatled_message_t *server_msg, size_t data_length) {
  puts("Hello!");
  if (!check_size(data_length, sizeof(beatled_hello_msg_t))) {
    return 1;
  }

  beatled_hello_msg_t *hello_msg = (beatled_hello_msg_t *)server_msg;
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);
  state_manager_set_state(STATE_REGISTERED);

  return 0;
}
