#include <lwip/dns.h>
#include <lwip/pbuf.h>
#include <pico/cyw43_arch.h>
#include <pico/unique_id.h>

#include "hal/network.h"
#include "hal/udp.h"

static bool server_address_resolved = false;
static ip_addr_t server_address;
static struct udp_pcb *server_udp_pcb;

static uint16_t udp_port_;
static uint16_t udp_server_port_;
static process_response_fn process_response_;

void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                const ip_addr_t *addr, u16_t port) {
  // printf("Received UDP datagram from port %d\n", port);
  size_t data_length = p->tot_len;
  void *server_msg = (void *)malloc(data_length);
  uint16_t copied_length = pbuf_copy_partial(p, server_msg, data_length, 0);

  if (copied_length > 0 && copied_length == data_length) {
    printf("Received: %d bytes\n", copied_length);
    printf("The string is: %.*s\n", copied_length, server_msg);

    if (process_response_(server_msg, data_length)) {
      puts("Error while queueing UDP message on event loop");
    }
    // if (!event_queue_add_message(event_server_message, server_msg,
    //                              data_length)) {
    //   puts("Error adding message to queue");
    // }
  } else {
    puts(" **** Error with msg...");
  }
  pbuf_free(p);
  puts("Done");
}

static void server_dns_found(const char *hostname, const ip_addr_t *ipaddr,
                             void *arg) {

  if (ipaddr != NULL) {
    printf("Host server found %s: %s\n", hostname, ipaddr_ntoa(ipaddr));
    server_address_resolved = true;
    ip_addr_copy(server_address, *ipaddr);
  } else {
    printf("Host server not found %s...\n", hostname);
  }
}

void resolve_server_address(const char *server_name) {
  err_t err;
  cyw43_arch_lwip_begin();

  err = dns_gethostbyname(server_name, &server_address, server_dns_found, NULL);
  cyw43_arch_lwip_end();

  if (err == ERR_INPROGRESS) {
    return;
  } else if (err == ERR_OK) {
    server_address_resolved = true;
  }
}

void resolve_server_address_blocking(const char *server_name) {
  resolve_server_address(server_name);
  while (!server_address_resolved) {
    sleep_ms(20);
  }
  puts("Server address resolved.");
}

void *udp_buffer_alloc(size_t msg_length) {

  struct pbuf *msg_pbuf = pbuf_alloc(PBUF_TRANSPORT, msg_length, PBUF_RAM);

  if (!msg_pbuf) {
    printf("failed to create pbuf\n");

    return NULL;
  }
  return msg_pbuf->payload;
}

void print_buffer_as_hex(uint8_t *buffer, int buffer_length) {
  int i;
  for (i = 0; i < buffer_length; i++) {
    if (i > 0)
      printf(":");
    printf("%02X", buffer[i]);
  }
}

int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload) {
  int err = 0;
  cyw43_arch_lwip_begin();

  struct pbuf *buffer = pbuf_alloc(PBUF_TRANSPORT, msg_length, PBUF_RAM);
  if (!buffer) {
    printf("failed to create pbuf\n");
    err = 1;
  } else {

    uint8_t *req = (uint8_t *)buffer->payload;
    memset(req, 0, msg_length);

    if (prepare_payload(buffer->payload, msg_length) != 0) {
      printf("Error preparing payload");
      err = 1;
    }

    if (udp_sendto(server_udp_pcb, buffer, &server_address, udp_server_port_) !=
        ERR_OK) {
      printf("Error sending message to %s:%u ...\n",
             ipaddr_ntoa(&server_address), udp_server_port_);
      err = 1;
    }

    printf("Sent message to %s:%u ... Command %x .. len %u\n",
           ipaddr_ntoa(&server_address), udp_server_port_, req[0], msg_length);
    printf("The string is ");
    print_buffer_as_hex(req, msg_length);
    printf("\n");
    pbuf_free(buffer);
  }
  cyw43_arch_lwip_end();

  return err;
}

// Perform initialisation
int init_udp_send_socket(uint16_t udp_port,
                         process_response_fn process_response) {
  udp_port_ = udp_port;
  process_response_ = process_response;

  cyw43_arch_lwip_begin();

  server_udp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (!server_udp_pcb) {
    printf("Failed to create bsp pcb\n");
    cyw43_arch_lwip_end();

    return 1;
  }

  // Bind to endpoint  IP_ADDR_BROADCAST IP_ANY_TYPE
  if (udp_bind(server_udp_pcb, IP_ANY_TYPE, udp_port_)) {
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
         ipaddr_ntoa(&server_udp_pcb->local_ip), udp_port_);
  cyw43_arch_lwip_end();

  return 0;
}

void start_udp(const char *server_name, uint16_t server_port, uint16_t udp_port,
               process_response_fn process_response) {
  init_udp_send_socket(udp_port, process_response);
  resolve_server_address_blocking(server_name);
  udp_server_port_ = server_port;
}

const uint32_t get_ip_address() {
  cyw43_arch_lwip_begin();

  // w0 on Pico W is the only network interface.
  // Network interfaces start with index 1
  struct netif *netif_ptr = netif_get_by_index(1);
  if (netif_ptr == NULL) {
    printf("Netif #%d is not defined\n", 1);
    cyw43_arch_lwip_end();
    return 0;
  }

  printf("Netif #%d is defined with name %s\n", 1, netif_ptr->name);

  const ip4_addr_t *addr = netif_ip4_addr(netif_ptr);
  cyw43_arch_lwip_end();

  return ip_addr_get_ip4_u32(addr);
}

void udp_print_all_ip_addresses() {
  cyw43_arch_lwip_begin();

  struct netif *netif_ptr;
  int idx = 1;
  char buf[IP4ADDR_STRLEN_MAX];
  while (1) {
    netif_ptr = netif_get_by_index(idx);
    if (netif_ptr == NULL) {
      // printf("Netif #%d is not defined\n", idx);
      cyw43_arch_lwip_end();
      return;
    }

    printf("Netif #%d is defined with name %s\n", idx, netif_ptr->name);

    const ip4_addr_t *netif_address = netif_ip4_addr(netif_ptr);

    if (ip4addr_ntoa_r(netif_address, buf, IP4ADDR_STRLEN_MAX) == NULL) {
      puts("Error converting address to char");
      cyw43_arch_lwip_end();

      return;
    }
    buf[15] = '\0';
    printf("Netif #%d ip address: %s\n", idx, buf);
    idx++;
  }
  cyw43_arch_lwip_end();
}