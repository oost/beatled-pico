#include <lwip/dns.h>
#include <lwip/pbuf.h>
#include <pico/cyw43_arch.h>
#include <pico/unique_id.h>

#include "hal/network.h"
#include "hal/udp.h"
#include "pico_udp.h"

static bool server_address_resolved = false;
ip_addr_t server_address;
uint16_t server_port;

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

void resolve_server_address(const char *remote_server_name,
                            uint16_t remote_server_port) {
  err_t err;
  cyw43_arch_lwip_begin();
  server_port = remote_server_port;

  err = dns_gethostbyname(remote_server_name, &server_address, server_dns_found,
                          NULL);
  cyw43_arch_lwip_end();

  if (err == ERR_INPROGRESS) {
    return;
  } else if (err == ERR_OK) {
    server_address_resolved = true;
  }
}

void resolve_server_address_blocking(const char *remote_server_name,
                                     uint16_t remote_server_port) {
  resolve_server_address(remote_server_name, remote_server_port);
  while (!server_address_resolved) {
    sleep_ms(20);
  }
  puts("Server address resolved.");
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