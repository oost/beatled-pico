#include "clock/clock.h"
#include "../blink/blink.h"

#include <time.h>
#include <pico/time.h>
#include <lwip/opt.h>
#include <lwip/apps/sntp.h>
#include <lwip/netif.h>
#include <lwip/dns.h>


#define SNTP_HOST_NAME "raspberrypi1.local"
// static void sntp_set_system_time_us(sec, us)

static absolute_time_t sntp_client_time_ref; 
static uint64_t sntp_server_time_ref_us;

static uint32_t sntp_server_tv_sec;
static uint32_t sntp_server_tv_nsec;

void
sntp_set_system_time(uint32_t sec, uint32_t usec)
{
  puts("Got SNTP response");

  sntp_client_time_ref = get_absolute_time();
  sntp_server_tv_sec = sec;
  sntp_server_tv_nsec = usec * 1000;
  sntp_server_time_ref_us = sec * 1000000 + usec;

  // blink(MESSAGE_BLINK_SPEED, MESSAGE_TIME_UPDATED);
  puts("Got time from SNTP server!");


  struct tm current_time_val;
  time_t current_time = (time_t)sec;

#if defined(_WIN32) || defined(WIN32)
  localtime_s(&current_time_val, &current_time);
#else
  localtime_r(&current_time, &current_time_val);
#endif

  char buf[32];
  strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &current_time_val);
  LWIP_PLATFORM_DIAG(("SNTP time: %s\n", buf));
}

absolute_time_t get_absolute_time_from_server_time(uint64_t server_time){
  return delayed_by_us(sntp_client_time_ref, server_time- sntp_server_time_ref_us);
}



void sntp_dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
  printf("Resolves SNTP server: %s, ip: %s\n", name, ipaddr_ntoa(ipaddr));
  sntp_setserver(0, ipaddr);
  
  // sntp_setoperatingmode(SNTP_OPMODE_POLL);

  // const char * ip_addr_str = "192.168.86.42";
  // ip_addr_t sntp_ip_addr;
  // ipaddr_aton	(	ip_addr_str, &sntp_ip_addr);
  // printf("Resolving %s: %s\n", ip_addr_str, ipaddr_ntoa(&sntp_ip_addr));
  // sntp_setserver(0, &sntp_ip_addr);

  sntp_init();
  
  // ip_addr_t sntp_server_address;
  // err_t err;
  // sntp_server_address = sntp_servers[sntp_current_server].addr;
  // err = (ip_addr_isany_val(sntp_server_address)) ? ERR_ARG : ERR_OK;

  puts("Initialized SNTP!");
}



void
sntp_sync_init(void)
{
  sntp_init();

  // const char* dnsname = SNTP_HOST_NAME;
  // ip_addr_t sntp_addr;
  

  // if (dns_gethostbyname(dnsname, &sntp_addr, sntp_dns_found, NULL) == ERR_OK) {
  //    return sntp_dns_found(dnsname, &sntp_addr, NULL);
  // } 
  // DNS reolution in progress
}


