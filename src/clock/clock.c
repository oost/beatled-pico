#include <time.h>

#include "lwip/opt.h"
#include "lwip/apps/sntp.h"
#include "lwip/netif.h"
#include "lwip/dns.h"

#include "clock/clock.h"
#include "../blink/blink.h"

void
sntp_set_system_time(u32_t sec)
{
  blink(MESSAGE_BLINK_SPEED, MESSAGE_TIME_UPDATED);
  puts("Got time from SNTP server!");
  char buf[32];
  struct tm current_time_val;
  time_t current_time = (time_t)sec;

#if defined(_WIN32) || defined(WIN32)
  localtime_s(&current_time_val, &current_time);
#else
  localtime_r(&current_time, &current_time_val);
#endif

  strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &current_time_val);
  LWIP_PLATFORM_DIAG(("SNTP time: %s\n", buf));
}


void sntp_dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
  printf("Adding SNTP server: %s\n", ipaddr_ntoa(ipaddr));
  sntp_setserver(0, ipaddr);

  sntp_init();
}



void
sntp_sync_init(void)
{
  puts("Resolving SNTP address");
  sntp_init();

  // const char* dnsname = SNTP_HOST_NAME;
  // ip_addr_t sntp_addr;
  
  // if (dns_gethostbyname(dnsname, &sntp_addr, sntp_dns_found, NULL) == ERR_OK) {
  //    return sntp_dns_found(dnsname, &sntp_addr, NULL);
  // }
  // puts("Error resolving SNTP address");
}
