// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>

// #define PORT 8080
// #define MAXLINE 1024

// // Driver code
// int main() {
//   int sockfd;
//   char buffer[MAXLINE];
//   char *hello = "Hello from server";
//   struct sockaddr_in servaddr, cliaddr;

//   // Creating socket file descriptor
//   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//     perror("socket creation failed");
//     exit(EXIT_FAILURE);
//   }

//   memset(&servaddr, 0, sizeof(servaddr));
//   memset(&cliaddr, 0, sizeof(cliaddr));

//   // Filling server information
//   servaddr.sin_family = AF_INET; // IPv4
//   servaddr.sin_addr.s_addr = INADDR_ANY;
//   servaddr.sin_port = htons(PORT);

//   // Bind the socket with the server address
//   if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
//   {
//     perror("bind failed");
//     exit(EXIT_FAILURE);
//   }

//   int len, n;

//   len = sizeof(cliaddr); // len is value/result

//   n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
//                (struct sockaddr *)&cliaddr, &len);
//   buffer[n] = '\0';
//   printf("Client : %s\n", buffer);
//   sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
//          (const struct sockaddr *)&cliaddr, len);
//   printf("Hello message sent.\n");

//   return 0;
// }

// int main(void) {
//   int socket_desc;
//   struct sockaddr_in server_addr, client_addr;
//   char server_message[2000], client_message[2000];
//   int client_struct_length = sizeof(client_addr);

//   // Clean buffers:
//   memset(server_message, '\0', sizeof(server_message));
//   memset(client_message, '\0', sizeof(client_message));

//   // Create UDP socket:
//   socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

//   if (socket_desc < 0) {
//     printf("Error while creating socket\n");
//     return -1;
//   }
//   printf("Socket created successfully\n");

//   // Set port and IP:
//   server_addr.sin_family = AF_INET;
//   server_addr.sin_port = htons(2000);
//   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

//   // Bind to the set port and IP:
//   if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr))
//   <
//       0) {
//     printf("Couldn't bind to the port\n");
//     return -1;
//   }
//   printf("Done with binding\n");

//   printf("Listening for incoming messages...\n\n");

//   // Receive client's message:
//   if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
//                (struct sockaddr *)&client_addr, &client_struct_length) < 0) {
//     printf("Couldn't receive\n");
//     return -1;
//   }
//   printf("Received message from IP: %s and port: %i\n",
//          inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

//   printf("Msg from client: %s\n", client_message);

//   // Respond to client:
//   strcpy(server_message, client_message);

//   if (sendto(socket_desc, server_message, strlen(server_message), 0,
//              (struct sockaddr *)&client_addr, client_struct_length) < 0) {
//     printf("Can't send\n");
//     return -1;
//   }

//   // Close the socket:
//   close(socket_desc);

//   return 0;
// }

#include "hal/udp.h"

void resolve_server_address(const char *server_name) {}
void resolve_server_address_blocking(const char *server_name) {}

// Perform initialisation
int init_server_udp_pcb(uint16_t udp_port, uint16_t udp_server_port,
                        process_response_fn process_response) {
  return 1;
}
const uint32_t *get_ip_address() { return NULL; }
void udp_print_all_ip_addresses() {}

int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload) {
  return 1;
}
