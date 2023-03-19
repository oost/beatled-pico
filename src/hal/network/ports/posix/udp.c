#include <arpa/inet.h>
#include <errno.h>
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
#include "hal/udp.h"

#define MAXLINE 1024

pthread_t udp_thread_;

int udp_socket_fd;
struct sockaddr_in server_addr;

typedef struct pbuf {
  void *payload;
} pbuf;

typedef struct udp_parameters {
  const char *server_name;
  uint16_t server_port;
  uint16_t udp_port;
  process_response_fn process_response;
} udp_parameters_t;

int create_udp_socket(udp_parameters_t *udp_params) {
  struct sockaddr_in *addr;
  struct sockaddr_in device_addr;
  if (resolve_server_address_blocking(udp_params->server_name, &server_addr)) {
    perror("socket creation failed");
    return 1;
  }
  addr = (struct sockaddr_in *)&server_addr;
  addr->sin_port = htons(udp_params->server_port);

  if ((udp_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket creation failed");
    return 1;
  }

  // Filling server information
  device_addr.sin_family = AF_INET; // IPv4
  device_addr.sin_addr.s_addr = INADDR_ANY;
  device_addr.sin_port = htons(udp_params->udp_port);

  // Bind the socket with the server address
  if (bind(udp_socket_fd, (const struct sockaddr *)&device_addr,
           sizeof(device_addr)) < 0) {
    perror("bind failed");
    return 1;
  }
  return 0;
}

void *udp_socket_listen(void *data) {
  udp_parameters_t *params = (udp_parameters_t *)data;

  char buffer[MAXLINE];
  int recvlen; /* # bytes received */

  struct sockaddr_in remaddr;          /* remote address */
  socklen_t addrlen = sizeof(remaddr); /* length of addresses */

  printf("Waiting on port %d\n", params->udp_port);
  for (;;) {
    recvlen = recvfrom(udp_socket_fd, buffer, MAXLINE - 1, 0,
                       (struct sockaddr *)&remaddr, &addrlen);
    printf("Received %d bytes\n", recvlen);
    if (recvlen > 0) {
      buffer[recvlen] = 0;
      // printf("Received message: \"%s\"\n", buffer);
      void *server_msg = (void *)malloc(recvlen);
      memcpy(server_msg, buffer, recvlen);
      if ((params->process_response)(server_msg, recvlen)) {
        puts("Error while queueing UDP message on event loop");
      }
    }
  }

  return NULL;
}

void start_udp(const char *server_name, uint16_t server_port, uint16_t udp_port,
               process_response_fn process_response) {
  udp_parameters_t *params = malloc(sizeof(udp_parameters_t));
  params->udp_port = udp_port;
  params->process_response = process_response;
  params->server_name = server_name;
  params->server_port = server_port;

  if (create_udp_socket(params)) {
    perror("Error creating sockets");
    return;
  }

  pthread_create(&udp_thread_, NULL, &udp_socket_listen, params);
}

const uint32_t get_ip_address() {
  unsigned char ip_address[15];
  int fd;
  struct ifreq ifr;

  /*AF_INET - to define network interface IPv4*/
  /*Creating soket for it.*/
  fd = socket(AF_INET, SOCK_DGRAM, 0);

  /*AF_INET - to define IPv4 Address type.*/
  ifr.ifr_addr.sa_family = AF_INET;

  /*eth0 - define the ifr_name - port name
  where network attached.*/
  memcpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

  /*Accessing network interface information by
  passing address using ioctl.*/
  ioctl(fd, SIOCGIFADDR, &ifr);
  /*closing fd*/
  close(fd);

  /*Extract IP Address*/
  strcpy((void *)ip_address,
         inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

  printf("System IP Address is: %s\n", ip_address);
  return 0;
}

void udp_print_all_ip_addresses() {
  struct ifaddrs *ifaddr;
  int family, s;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    exit(EXIT_FAILURE);
  }

  /* Walk through linked list, maintaining head pointer so we
     can free list later. */

  for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;

    family = ifa->ifa_addr->sa_family;

    /* Display interface name and family (including symbolic
       form of the latter for the common families). */

    printf("%-8s %s (%d)\n", ifa->ifa_name,
           (family == AF_INET)    ? "AF_INET"
           : (family == AF_INET6) ? "AF_INET6"
                                  : "???",
           family);

    /* For an AF_INET* interface address, display the address. */

    if (family == AF_INET || family == AF_INET6) {
      s = getnameinfo(ifa->ifa_addr,
                      (family == AF_INET) ? sizeof(struct sockaddr_in)
                                          : sizeof(struct sockaddr_in6),
                      host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
        printf("getnameinfo() failed: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
      }

      printf("\t\taddress: <%s>\n", host);
    }
  }

  freeifaddrs(ifaddr);
}

int sendall(int socket_fd, char *data_buffer, size_t *data_length,
            const struct sockaddr_in *recipient_addr) {
  int total = 0;                // how many bytes we've sent
  int bytesleft = *data_length; // how many we have left to send
  int n;

  while (total < *data_length) {
    n = sendto(socket_fd, data_buffer + total, bytesleft, 0,
               (const struct sockaddr *)recipient_addr,
               sizeof(*recipient_addr));
    if (n == -1) {
      printf("UDP Send: %s\n", strerror(errno));
      break;
    }
    total += n;
    bytesleft -= n;
  }

  *data_length = total; // return number actually sent here

  return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

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

    if (sendall(udp_socket_fd, buffer->payload, &msg_length, &server_addr)) {
      printf("Error sending message\n");
    }
  }

  char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
  const struct sockaddr_in *recipient_addr =
      (const struct sockaddr_in *)&server_addr;
  inet_ntop(AF_INET, &(recipient_addr->sin_addr), ip4, INET_ADDRSTRLEN);
  printf("Sent UDP request to %s:%d\n", ip4, ntohs(recipient_addr->sin_port));

  free(buffer->payload);
  free(buffer);

  return err;
}
