#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#define UDP_SERVER_PORT 9090
#define UDP_PORT 8765

#include <lwip/pbuf.h>
#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>

void resolve_server_address();
void resolve_server_address_blocking();

// Perform initialisation
int init_server_udp_pcb();
const ip4_addr_t *get_ip_address();
void udp_print_all_ip_addresses();

typedef int (*prepare_payload_fn)(struct pbuf *buffer, size_t size);

int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload);

#endif // UDP_SERVER_H