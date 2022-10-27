#include <pico/cyw43_arch.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>

#include "blink/blink.h"
#include "clock/clock.h"
#include "command_queue/queue.h"
#include "core0.h"
#include "core1.h"
#include "state_manager/state_manager.h"
#include "udp_server/udp_server.h"
#include "wifi/wifi.h"
#include "ws2812/ws2812.h"

void core1_entry() {
  core1_init();
  core1_loop();
}

void init() {
  puts("- Starting STDIO");
  stdio_init_all();

  puts("- Starting State Manager");
  state_manager_init();

  puts("- Starting Wifi");
  wifi_init();
  wifi_check();

  puts("- Starting WS2812 Manager");
  led_init();

  puts("- Starting Command queue");
  command_queue_init();

  puts("- Sending Hello message via UDP");
  udp_print_all_ip_addresses();

  resolve_server_address_blocking();

  // puts("- Starting SNTP poll");
  // sntp_sync_init();

  puts("- Starting Beat Server");
  init_server_udp_pcb();
  send_hello_msg();
  send_time_sync_request();
}

void deinit() {
  puts("Deinit... Not sure how we got here");
  wifi_deinit();
}

int main(void) {
  printf("Starting on pico board %s\n", state_manager_get_unique_board_id());

  // Starting initialization sequence
  init();

  // Launch core 1
  multicore_launch_core1(core1_entry);

  // Do core 0 specific initialization
  core0_init();

  // Start infinite loop on core 0
  core0_loop();

  // Shouldn't ever get here but who knows...
  deinit();
  return 0;
}
