#include "time.h"
#include "beatled/protocol.h"
#include "clock/clock.h"
#include "command/utils.h"
#include "config/constants.h"
#include "hal/network.h"
#include "hal/udp.h"
#include "state_manager/state_manager.h"
#include "state_manager/states.h"

int prepare_time_request(void *buffer_payload, size_t buf_len) {
  if (buf_len != sizeof(beatled_message_time_request_t)) {
    printf("[ERR] Time request size mismatch: %zu vs %zu\n", buf_len,
           sizeof(beatled_message_time_request_t));
    return 1;
  }

  beatled_message_time_request_t *msg = buffer_payload;
  msg->base.type = BEATLED_MESSAGE_TIME_REQUEST;
  uint64_t orig_time = time_us_64();
  msg->orig_time = htonll(orig_time);
#if BEATLED_VERBOSE_LOG
  printf("[CMD] Sending time request, orig_time=%llu\n", orig_time);
#endif

  return 0;
}

int send_time_request() {
  return send_udp_request(sizeof(beatled_message_time_request_t),
                          &prepare_time_request);
}

int validate_time_msg(beatled_message_t *server_msg, size_t data_length,
                      uint64_t dest_time) {
  if (!check_size(data_length, sizeof(beatled_message_time_response_t))) {
    return 1;
  }
  return 0;
}

int process_time_msg(beatled_message_t *server_msg, size_t data_length,
                     uint64_t dest_time) {
  if (!check_size(data_length, sizeof(beatled_message_time_response_t))) {
    return 1;
  }
  beatled_message_time_response_t *time_resp_msg =
      (beatled_message_time_response_t *)server_msg;

  uint64_t orig_time = ntohll(time_resp_msg->orig_time);
  uint64_t recv_time = ntohll(time_resp_msg->recv_time);
  uint64_t xmit_time = ntohll(time_resp_msg->xmit_time);

  uint64_t delay = (dest_time - orig_time) - (xmit_time - recv_time);
  int64_t clock_offset = ((int64_t)(recv_time / 2) - (int64_t)(orig_time / 2)) +
                         ((int64_t)(xmit_time / 2) - (int64_t)(dest_time / 2));
#if BEATLED_VERBOSE_LOG
  printf("[CMD] Time sync: orig=%llu recv=%llu xmit=%llu dest=%llu\n",
         orig_time, recv_time, xmit_time, dest_time);
  printf("[CMD] Delay=%llu, offset=%lld\n", delay, clock_offset);
#endif

  set_server_time_offset(clock_offset);

  if (state_manager_get_state() < STATE_TIME_SYNCED) {
    if (!schedule_state_transition(STATE_TIME_SYNCED)) {
      BEATLED_FATAL("Failed to schedule transition to time synced state");
      return 1;
    }
  }

  return 0;
}