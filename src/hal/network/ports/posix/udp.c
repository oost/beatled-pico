#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "hal/udp.h"
#define MAXLINE 1024

pthread_t udp_thread;
typedef struct pbuf {
  void *payload;
} pbuf;

typedef struct udp_parameters {
  uint16_t udp_port;
  process_response_fn process_response;
} udp_parameters_t;

void resolve_server_address(const char *server_name) {
  struct addrinfo *result, *rp;
  int sfd, s;

  struct addrinfo hints = {0};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  s = getaddrinfo(server_name, NULL, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s (servername: %s)\n", gai_strerror(s),
            server_name);
    exit(EXIT_FAILURE);
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until a successful bind().
     If socket(2) (or bind(2)) fails, close the socket
     and try the next address. */

  char ipstr[INET6_ADDRSTRLEN];
  // struct sockaddr_in sa; // pretend this is loaded with something
  printf("Resolved %s:\n", server_name);
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    void *addr;
    char *ipver;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:
    if (rp->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(rp->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(result); /* No longer needed */
}
void resolve_server_address_blocking(const char *server_name) {
  resolve_server_address(server_name);
}

void *start_udp_server(void *data) {
  udp_parameters_t *params = (udp_parameters_t *)data;
  // udp_parameters params= (udp_parameters*data;

  int sockfd;
  char buffer[MAXLINE];
  int recvlen; /* # bytes received */
  struct sockaddr_in ownaddr;
  struct sockaddr_in remaddr;          /* remote address */
  socklen_t addrlen = sizeof(remaddr); /* length of addresses */

  // Creating socket file descriptor

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket creation failed");
    return NULL;
  }

  // Filling server information
  ownaddr.sin_family = AF_INET; // IPv4
  ownaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  ownaddr.sin_port = htons(params->udp_port);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&ownaddr, sizeof(ownaddr)) < 0) {
    perror("bind failed");
    return NULL;
  }

  for (;;) {
    printf("waiting on port %d\n", params->udp_port);
    recvlen = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&remaddr,
                       &addrlen);
    printf("received %d bytes\n", recvlen);
    if (recvlen > 0) {
      buffer[recvlen] = 0;
      printf("received message: \"%s\"\n", buffer);
    }
  }

  return NULL;
}

void start_udp_thread(uint16_t udp_port, process_response_fn process_response) {
  udp_parameters_t *params = malloc(sizeof(udp_parameters_t));

  params->udp_port = udp_port;
  params->process_response = process_response;
  pthread_create(&udp_thread, NULL, &start_udp_server, params);
}

// Perform initialisation
int init_server_udp_pcb(uint16_t udp_port, uint16_t udp_server_port,
                        process_response_fn process_response) {
  udp_parameters_t params = {.udp_port = udp_port,
                             .process_response = process_response};
  // start_udp_server(&params);
  start_udp_thread(udp_port, process_response);
  return 0;
}
const uint32_t *get_ip_address() { return NULL; }
void udp_print_all_ip_addresses() {}

int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload) {
  int err = 0;
  pbuf *buffer = (pbuf *)malloc(sizeof(pbuf));

  buffer->payload = malloc(msg_length);
  if (!buffer->payload) {
    printf("failed to create puffer\n");
    err = 1;
  } else {

    uint8_t *req = (uint8_t *)buffer->payload;
    memset(req, 0, msg_length);

    if (prepare_payload(buffer->payload, msg_length) != 0) {
      printf("Error preparing payload");
      err = 1;
    }

    // if (udp_sendto(server_udp_pcb, buffer, &server_address, udp_server_port_)
    // !=
    //     ERR_OK) {
    //   printf("Error sending message to %s:%u ...\n",
    //          ipaddr_ntoa(&server_address), udp_server_port_);
    //   err = 1;
  }

  // printf("Sent message to %s:%u ... Command %x .. len %u\n",
  //        ipaddr_ntoa(&server_address), udp_server_port_, req[0], msg_length);
  // printf("The string is ");
  // print_buffer_as_hex(req, msg_length);
  printf("Sent UDP request\n");
  free(buffer->payload);
  free(buffer);

  return err;
}
