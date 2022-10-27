#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#define UDP_SERVER_PORT 9090
#define UDP_PORT 8765

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

int send_hello_msg();
int send_time_sync_request();

void resolve_server_address();
void resolve_server_address_blocking();

// Perform initialisation
int init_server_udp_pcb();
const ip4_addr_t *get_ip_address();
void udp_print_all_ip_addresses();

#endif // UDP_SERVER_H