#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#define BSP_PORT 8765

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

typedef struct BSP_T_ {
    struct udp_pcb *bsp_pcb;
} BSP_T;

// NTP data received
void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);

int send_hello();

// Perform initialisation
BSP_T *bsp_init(void);
const ip4_addr_t* get_ip_address();
void print_all_ip_addresses();


#endif // UDP_SERVER_H