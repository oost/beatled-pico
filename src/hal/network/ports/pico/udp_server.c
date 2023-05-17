#include <lwip/dns.h>
#include <lwip/pbuf.h>
#include <pico/cyw43_arch.h>
#include <pico/unique_id.h>

#include "hal/network.h"
#include "hal/udp.h"
#include "pico_udp.h"

struct udp_pcb *server_udp_pcb;
static process_response_fn process_response_;

static uint32_t msg_id = 0;

void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                const ip_addr_t *addr, u16_t port) {
  // printf("Received UDP datagram from port %d\n", port);

  // TODO: Accept only DNS requests from host ip_addr_cmp

  size_t data_length = p->tot_len;
  void *server_msg = (void *)malloc(data_length);
  // printf("Allocated memory at %x\n", server_msg);

  // Copy message to our buffer and free the pbuf.
  uint16_t copied_length = pbuf_copy_partial(p, server_msg, data_length, 0);
  // printf("Received UDP message of size %d. First byte: %d. Sender: %s\n",
  //        data_length, ((char *)server_msg)[0], ipaddr_ntoa(addr));

  pbuf_free(p);

  int server_msg_enqueue_error = 1;

  if (copied_length > 0 && copied_length == data_length) {
    // printf("Received: %d bytes\n", copied_length);
    // printf("The string is: %.*s\n", copied_length, server_msg);

    // Let's process the message
    // puts("Queueing message");
    server_msg_enqueue_error = process_response_(server_msg, data_length);
    if (server_msg_enqueue_error) {
      puts("Error while queueing UDP message on event loop");
    }
  } else {
    puts(" **** Error with msg...");
  }

  if (server_msg_enqueue_error) {
    // We didn't manage to enqueue the message so we are still the owner. Let's
    // free it...
    printf("Something went wrong... Freeing memory at %x\n", server_msg);
    free(server_msg);
  }

  // puts("Exiting UDP handler");
}

// Perform initialisation
int init_udp_socket(uint16_t udp_port) {
  cyw43_arch_lwip_begin();

  server_udp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (!server_udp_pcb) {
    printf("Failed to create bsp pcb\n");
    cyw43_arch_lwip_end();

    return 1;
  }

  // Bind to endpoint  IP_ADDR_BROADCAST IP_ANY_TYPE
  if (udp_bind(server_udp_pcb, IP_ANY_TYPE, udp_port)) {
    printf("Failed to bind pcb\n");
    udp_remove(server_udp_pcb);
    cyw43_arch_lwip_end();

    return 1;
  }

  // ip_set_option(server_udp_pcb, SOF_BROADCAST);
  ip_set_option(server_udp_pcb, IP_SOF_BROADCAST_RECV);

  // Add callback
  udp_recv(server_udp_pcb, dgram_recv, NULL);
  printf("Set up UDP callback on %s:%d\n",
         ipaddr_ntoa(&server_udp_pcb->local_ip), udp_port);
  cyw43_arch_lwip_end();

  return 0;
}

void start_udp(const char *server_name, uint16_t server_port, uint16_t udp_port,
               process_response_fn process_response) {
  process_response_ = process_response;
  resolve_server_address_blocking(server_name, server_port);
  init_udp_socket(udp_port);
}
