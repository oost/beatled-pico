#include "udp_server/udp_server.h"
#include "ws2812/ws2812.h"
#include "command/constants.h"
#include "command_queue/queue.h"


// NTP data received
void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    printf("Received UDP datagram on port %d\n", port);

    BSP_T *state = (BSP_T *)arg;
    uint8_t instruction = pbuf_get_at(p, 0);
    char* msg = malloc((COMMAND_MAX_LEN) * sizeof(char));
    u16_t msglen = pbuf_copy_partial(p, msg, COMMAND_MAX_LEN, 0);
    if (msglen) {
        if (msglen == COMMAND_MAX_LEN) {
          puts("Received more than 128 bytes. Discarding the extra bytes...");
        } else {
          printf("Received: %d bytes\n", msglen);
        }
        fwrite(msg, 1, msglen, stdout);
        // msg[l] = '\0';
        // printf("Received: %s\n", msg);
        command_queue_add_message(msg, msglen, port);
    } else { 
        puts("Error with msg");
    }

    pbuf_free(p);
}



int send_hello() {
  const char * ip_addr_string = "192.168.86.24";
  ip_addr_t *addr = malloc(sizeof(ip_addr_t));
  u16_t port = 8765;
  
  if (!ip4addr_aton(ip_addr_string, addr ) ) 
  {
      printf("failed to create ip addr\n");
      return 0;
  }	

  struct udp_pcb * upcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (!upcb)
  {
      printf("failed to create pcb\n");
      return 0;
  }
  const char* msg = "Hillo World!!!";

  struct pbuf* hello_pbuf = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM );	
  if (!hello_pbuf)
  {
      printf("failed to create pbuf\n");
      return 0;
  }
  if (pbuf_take(hello_pbuf,msg, sizeof(msg)) != ERR_OK) 
  {
      printf("failed to copy into pbuf\n");
      return 0;
  }

  udp_sendto(upcb, hello_pbuf, addr, port);
  pbuf_free(hello_pbuf);
  free(addr);
}

// Perform initialisation
BSP_T *bsp_init(void)
{
    BSP_T *state = calloc(1, sizeof(BSP_T));
    if (!state)
    {
        printf("Failed to allocate state\n");
        return NULL;
    }

    state->bsp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!state->bsp_pcb)
    {
        printf("Failed to create pcb\n");
        free(state);
        return NULL;
    }

    // Bind to endpoint  
    if (udp_bind(state->bsp_pcb, IP_ANY_TYPE, BSP_PORT))
    {
        printf("Failed to bind pcb\n");
        free(state);
        return NULL;
    }

    // Add callback
    udp_recv(state->bsp_pcb, dgram_recv, state);
    printf("Set up UDP callback on %s:%d\n", ipaddr_ntoa(&state->bsp_pcb->local_ip), BSP_PORT);

    return state;
}

const ip4_addr_t* get_ip_address() 
{
  // w0 on Pico W is the only network interface. 
  // Network interfaces start with index 1
  struct netif* netif_ptr = netif_get_by_index(1);
  if (netif_ptr == NULL) 
  {
    printf("Netif #%d is not defined\n", 1);
    return NULL;
  }
    
  printf("Netif #%d is defined with name %s\n", 1, netif_ptr->name);

  return netif_ip4_addr(netif_ptr);
}


void print_all_ip_addresses() {
  struct netif* netif_ptr;
  int idx = 1;
  char buf[IP4ADDR_STRLEN_MAX];
  while(1) {
    netif_ptr = netif_get_by_index(idx);
    if (netif_ptr == NULL) 
    {
      printf("Netif #%d is not defined\n", idx);
      return;
    }
     
    printf("Netif #%d is defined with name %s\n", idx, netif_ptr->name);

    const ip4_addr_t* netif_address = netif_ip4_addr(netif_ptr);

    if(ip4addr_ntoa_r(netif_address, buf, IP4ADDR_STRLEN_MAX)	== NULL)
    {
      puts("Error converting address to char");
      return;
    }
    buf[15] = '\0';
    printf("Netif #%d ip address: %s\n", idx, buf);
    idx++;
  } 

  
}