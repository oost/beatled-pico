#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>

#include "blink/blink.h"
#include "clock/clock.h"
#include "constants.h"
#include "core0.h"
#include "core1.h"
#include "event_queue/queue.h"
#include "intercore_queue.h"
#include "started.h"
#include "state_manager/state.h"
#include "state_manager/state_manager.h"
#include "udp_server/udp_server.h"
#include "wifi/wifi.h"
#include "ws2812/ws2812.h"

#include "command/hello/hello.h"
#include "command/tempo/tempo.h"
#include "command/time/time.h"

queue_t intercore_command_queue;

int enter_started_state() {
  printf("Starting on pico board %s\n", state_manager_get_unique_board_id());

  queue_init(&intercore_command_queue, sizeof(state_update_t),
             MAX_INTERCORE_QUEUE_COUNT);

  puts("- Starting STDIO");
  stdio_init_all();

  puts("- Starting State Manager");
  state_manager_init();

  puts("- Starting Wifi");
  wifi_init();
  wifi_check();

  puts("- Starting WS2812 Manager");
  led_init();

  puts("- Starting Event queue");
  event_queue_init();

  puts("- Sending Hello message via UDP");
  udp_print_all_ip_addresses();

  resolve_server_address_blocking();
  sleep_ms(500);

  // puts("- Starting SNTP poll");
  // sntp_sync_init();

  puts("- Starting Beat Server");
  init_server_udp_pcb();

  state_manager_set_state(STATE_INITIALIZED);
  return 0;
}

int exit_started_state() { return 0; }