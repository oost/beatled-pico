#include "time.h"
#include "beatled/protocol.h"
#include "clock/clock.h"
#include "command/utils.h"
#include "hal/network.h"
#include "hal/udp.h"
#include "state_manager/state_manager.h"
#include "state_manager/states/states.h"

int prepare_time_request(void *buffer_payload, size_t buf_len) {
  if (buf_len != sizeof(beatled_message_time_request_t)) {
    printf("Error sizes don't match %zu, %zu", buf_len,
           sizeof(beatled_message_time_request_t));
    return 1;
  }

  beatled_message_time_request_t *msg = buffer_payload;
  msg->base.type = BEATLED_MESSAGE_TIME_REQUEST;
  uint64_t orig_time = time_us_64();
  printf("Sending time request. \n - orig_time: %llu / %llx\n", orig_time,
         orig_time);

  msg->orig_time = htonll(orig_time);

  printf("Sending time request. \n - orig_time: %llu / %llx\n", msg->orig_time,
         msg->orig_time);

  return 0;
}

int send_time_request() {
  return send_udp_request(sizeof(beatled_message_time_request_t),
                          &prepare_time_request);
}

int validate_time_msg(beatled_message_t *server_msg, size_t data_length,
                      uint64_t dest_time) {
  puts("Time!");
  if (!check_size(data_length, sizeof(beatled_message_time_response_t))) {
    return 1;
  }
  return 0;
}

int process_time_msg(beatled_message_t *server_msg, size_t data_length,
                     uint64_t dest_time) {
  puts("Time!");
  if (!check_size(data_length, sizeof(beatled_message_time_response_t))) {
    return 1;
  }
  beatled_message_time_response_t *time_resp_msg =
      (beatled_message_time_response_t *)server_msg;

  uint64_t orig_time = ntohll(time_resp_msg->orig_time);
  uint64_t recv_time = ntohll(time_resp_msg->recv_time);
  uint64_t xmit_time = ntohll(time_resp_msg->xmit_time);

  uint64_t delay = (dest_time - orig_time) - (xmit_time - recv_time);
  int64_t clock_offset =
      ((signed)(recv_time - orig_time) + (signed)(xmit_time - dest_time)) / 2;
  printf(
      "Got times\n - orig: %llu\n - recv: %llu\n - xmit: %llu\n - dest: %llu\n",
      orig_time, recv_time, xmit_time, dest_time);
  printf(
      "Got times\n - orig: %llu\n - recv: %llu\n - xmit: %llu\n - dest: %llu\n",
      time_resp_msg->orig_time, time_resp_msg->recv_time,
      time_resp_msg->xmit_time, dest_time);
  printf("Delay %llu\n offset: %lld\n", delay, clock_offset);

  set_server_time_offset(clock_offset);

  if (!schedule_state_transition(STATE_TIME_SYNCED)) {
    puts("- Can't schedule transition to time synced state. Fatal error.");
    exit(1);
  }

  return 0;
}