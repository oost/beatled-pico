#include <lwip/dns.h>
#include <pico/unique_id.h>

#include "beatled/protocol.h"
#include "event_queue/queue.h"
#include "udp_server/udp_server.h"
#include "utils/network.h"
#include "ws2812/ws2812.h"

static bool server_address_resolved = false;
static ip_addr_t server_address;
static struct udp_pcb *server_udp_pcb;

// NTP data received
void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                const ip_addr_t *addr, u16_t port) {
  // printf("Received UDP datagram from port %d\n", port);
  size_t data_length = p->tot_len;
  void *server_msg = (void *)malloc(data_length);
  uint16_t copied_length = pbuf_copy_partial(p, server_msg, data_length, 0);

  if (copied_length > 0 && copied_length == data_length) {
    printf("Received: %d bytes\n", copied_length);
    printf("The string is: %.*s\n", copied_length, server_msg);

    if (!event_queue_add_message(event_server_message, server_msg,
                                 data_length)) {
      puts("Error adding message to queue");
    }
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

void resolve_server_address() {
  err_t err;

  err = dns_gethostbyname(SERVER_NAME, &server_address, server_dns_found, NULL);
  if (err == ERR_INPROGRESS) {
    return;
  } else if (err == ERR_OK) {
    server_address_resolved = true;
  }
}

void resolve_server_address_blocking() {
  resolve_server_address();
  while (!server_address_resolved) {
    sleep_ms(20);
  }
  puts("Server address resolved.");
}

int send_udp_request(beatled_message_t *msg, size_t msg_length) {

  struct pbuf *msg_pbuf = pbuf_alloc(PBUF_TRANSPORT, msg_length + 1, PBUF_RAM);

  if (!msg_pbuf) {
    printf("failed to create pbuf\n");
    return 1;
  }

  if (pbuf_take(msg_pbuf, msg, sizeof(msg)) != ERR_OK) {
    printf("failed to copy into pbuf\n");
    pbuf_free(msg_pbuf);
    return 1;
  }

  if (udp_sendto(server_udp_pcb, msg_pbuf, &server_address, UDP_SERVER_PORT) !=
      ERR_OK) {
    pbuf_free(msg_pbuf);
    printf("Error sending message to %s:%u ...\n", ipaddr_ntoa(&server_address),
           UDP_SERVER_PORT);
    return 1;
  }

  pbuf_free(msg_pbuf);
  printf("Sent message to %s:%u ... Command %c\n", ipaddr_ntoa(&server_address),
         UDP_SERVER_PORT, (char *)msg);
  return 0;
}

int send_hello_msg() {

  beatled_hello_msg_t msg;
  msg.base.type = eBeatledHello;
  pico_get_unique_board_id_string(msg.board_id, count_of(msg.board_id));

  return send_udp_request((beatled_message_t *)&msg, sizeof(msg));
}

int send_time_sync_request() {
  beatled_time_req_msg_t msg;
  msg.base.type = eBeatledTime;
  msg.orig_time = htonll(time_us_64());

  return send_udp_request((beatled_message_t *)&msg, sizeof(msg));
}

// Perform initialisation
int init_server_udp_pcb() {

  server_udp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (!server_udp_pcb) {
    printf("Failed to create bsp pcb\n");
    return 1;
  }

  // Bind to endpoint  IP_ADDR_BROADCAST IP_ANY_TYPE
  if (udp_bind(server_udp_pcb, IP_ANY_TYPE, UDP_PORT)) {
    printf("Failed to bind pcb\n");
    udp_remove(server_udp_pcb);
    return 1;
  }

  ip_set_option(server_udp_pcb, SOF_BROADCAST);
  ip_set_option(server_udp_pcb, IP_SOF_BROADCAST_RECV);

  // Add callback
  udp_recv(server_udp_pcb, dgram_recv, NULL);
  printf("Set up UDP callback on %s:%d\n",
         ipaddr_ntoa(&server_udp_pcb->local_ip), UDP_PORT);

  return 0;
}

const ip4_addr_t *get_ip_address() {
  // w0 on Pico W is the only network interface.
  // Network interfaces start with index 1
  struct netif *netif_ptr = netif_get_by_index(1);
  if (netif_ptr == NULL) {
    printf("Netif #%d is not defined\n", 1);
    return NULL;
  }

  printf("Netif #%d is defined with name %s\n", 1, netif_ptr->name);

  return netif_ip4_addr(netif_ptr);
}

void udp_print_all_ip_addresses() {
  struct netif *netif_ptr;
  int idx = 1;
  char buf[IP4ADDR_STRLEN_MAX];
  while (1) {
    netif_ptr = netif_get_by_index(idx);
    if (netif_ptr == NULL) {
      // printf("Netif #%d is not defined\n", idx);
      return;
    }

    printf("Netif #%d is defined with name %s\n", idx, netif_ptr->name);

    const ip4_addr_t *netif_address = netif_ip4_addr(netif_ptr);

    if (ip4addr_ntoa_r(netif_address, buf, IP4ADDR_STRLEN_MAX) == NULL) {
      puts("Error converting address to char");
      return;
    }
    buf[15] = '\0';
    printf("Netif #%d ip address: %s\n", idx, buf);
    idx++;
  }
}