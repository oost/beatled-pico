#ifndef HAL__UDP_H
#define HAL__UDP_H

#ifdef __cplusplus
extern "C" {
#endif
// #include <lwip/pbuf.h>
#include <stdlib.h>

typedef int (*prepare_payload_fn)(void *buffer_payload, size_t size);
typedef int (*process_response_fn)(void *buffer_payload, size_t size);

void resolve_server_address(const char *server_name);
void resolve_server_address_blocking(const char *server_name);

// Perform initialisation
int init_server_udp_pcb(uint16_t udp_port, uint16_t udp_server_port,
                        process_response_fn process_response);
const uint32_t *get_ip_address();
void udp_print_all_ip_addresses();

int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload);

#ifdef __cplusplus
}
#endif

#endif // HAL__UDP_H