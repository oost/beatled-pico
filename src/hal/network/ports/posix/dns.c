#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dns.h"

int resolve_server_address(const char *server_name,
                           struct sockaddr_in *server_addr) {
  struct addrinfo *result, *rp;
  int sfd, s;

  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  s = getaddrinfo(server_name, NULL, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "[ERR] getaddrinfo: %s (server: %s)\n", gai_strerror(s),
            server_name);
    return 1;
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until a successful bind().
     If socket(2) (or bind(2)) fails, close the socket
     and try the next address. */

  char ipstr[INET6_ADDRSTRLEN];
  // struct sockaddr_in sa; // pretend this is loaded with something
  printf("[NET] Resolved %s\n", server_name);
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    if (rp->ai_family != AF_INET) {
      // Shouldnt ever be the case...
      continue;
    }

    void *addr;
    char *ipver;

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
    addr = &(ipv4->sin_addr);
    ipver = "IPv4";

    // convert the IP to a string and print it:
    inet_ntop(rp->ai_family, addr, ipstr, sizeof ipstr);
    printf("[NET]   %s: %s\n", ipver, ipstr);
    break;
  }

  memcpy(server_addr, rp->ai_addr, sizeof(struct sockaddr_in));

  freeaddrinfo(result); /* No longer needed */
  return 0;
}

int resolve_server_address_blocking(const char *server_name,
                                    struct sockaddr_in *server_addr) {
  return resolve_server_address(server_name, server_addr);
}