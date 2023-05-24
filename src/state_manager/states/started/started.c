#include "hal/queue.h"
#include <stdio.h>
#include <stdlib.h>

#include "config/constants.h"
#include "event/event_queue.h"
#include "hal/board.h"
#include "hal/process.h"
#include "hal/udp.h"
#include "hal/wifi.h"

#include "hal/registry.h"
#include "process/intercore_queue.h"
#include "state_manager/state_manager.h"
#include "state_manager/states/started.h"

hal_queue_handle_t intercore_command_queue;

int enter_started_state() {
  // board_id_handle_t board_id_ptr = get_unique_board_id();
  // printf("Starting on pico board %s\n", state_manager_get_unique_board_id());

  puts("- Initializing registry");
  registry_init();

  puts("- Initializing event queue");
  event_queue_init();

  puts("- Initializing intercore queue");
  intercore_command_queue =
      hal_queue_init(sizeof(intercore_message_t), MAX_INTERCORE_QUEUE_COUNT);

  puts("- Starting STDIO");
  hal_stdio_init();

  puts("- Starting Wifi");
  wifi_init();
  wifi_check(WIFI_SSID, WIFI_PASSWORD);

  puts("- Starting IP stack");
  udp_print_all_ip_addresses();
  // get_ip_address();
  // resolve_server_address_blocking(BEATLED_SERVER_NAME);

  sleep_ms(500);

  puts("- Starting Beat Server");
  start_udp(BEATLED_SERVER_NAME, UDP_SERVER_PORT, UDP_PORT,
            &add_payload_to_event_queue);

  if (!schedule_state_transition(STATE_INITIALIZED)) {
    puts("- Can't schedule transition to initialized state. Fatal error.");
    exit(1);
  }

  return 0;
}

int exit_started_state() { return 0; }