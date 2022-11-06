#include "hal/queue.h"
#include <stdio.h>
#include <stdlib.h>

#include "clock.h"
#include "constants.h"
#include "event_queue/queue.h"
#include "hal/blink.h"
#include "hal/board.h"
#include "hal/process.h"
#include "hal/queue.h"
#include "hal/udp.h"
#include "hal/wifi.h"
#include "process/core0.h"
#include "process/core1.h"
#include "process/intercore_queue.h"
#include "started.h"
#include "state.h"
#include "state_manager.h"
#include "ws2812.h"

#include "command/hello/hello.h"
#include "command/tempo/tempo.h"
#include "command/time/time.h"

hal_queue_handle_t intercore_command_queue;

int enter_started_state() {
  // board_id_handle_t board_id_ptr = get_unique_board_id();

  // printf("Starting on pico board %s\n", state_manager_get_unique_board_id());

  puts("- Starting STDIO");
  hal_stdio_init();

  intercore_command_queue =
      hal_queue_init(sizeof(state_update_t), MAX_INTERCORE_QUEUE_COUNT);

  puts("- Starting Wifi");
  wifi_init();
  wifi_check(WIFI_SSID, WIFI_PASSWORD);

  puts("- Starting WS2812 Manager");
  led_init();

  puts("- Starting Event queue");
  event_queue_init();

  puts("- Sending Hello message via UDP");
  udp_print_all_ip_addresses();

  resolve_server_address_blocking(SERVER_NAME);
  sleep_ms(500);

  // puts("- Starting SNTP poll");
  // sntp_sync_init();

  puts("- Starting Beat Server");
  init_server_udp_pcb(UDP_PORT, UDP_SERVER_PORT, &add_payload_to_event_queue);

  state_manager_set_state(STATE_INITIALIZED);
  return 0;
}

int exit_started_state() { return 0; }