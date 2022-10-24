#include "wifi.h"
#include "blink/blink.h"
#include "constants.h"
#include <pico/cyw43_arch.h>

int wifi_connect() {
  if (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASSWORD,
                                       CYW43_AUTH_WPA2_AES_PSK)) {
    blink(ERROR_BLINK_SPEED, ERROR_WIFI);
    printf("Failed to connect to WIFI\n");
    return 1;
  }
  printf("Connected to %s\n", WIFI_SSID);
  blink(MESSAGE_BLINK_SPEED, MESSAGE_CONNECTED);
  return 0;
}

void wifi_check() {
  if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) != CYW43_LINK_JOIN) {
    while (1) {
      if (!wifi_connect()) {
        return;
      }
    }
  }
}

void wifi_init() {
  if (cyw43_arch_init()) {
    printf("WiFi init failed");
    return;
  }
  cyw43_arch_enable_sta_mode();
}

void wifi_deinit() { cyw43_arch_deinit(); }