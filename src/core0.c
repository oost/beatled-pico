#include <stdio.h>
#include "pico/stdlib.h"
#include "core0.h"
// #include "hardware/gpio.h"
// #include "hardware/timer.h"
// #include "hardware/clocks.h"

#include "constants.h"
#include "pico/cyw43_arch.h"
#include "ws2812/ws2812.h"
#include "clock/clock.h"
#include "blink/blink.h"
#include "udp_server/udp_server.h"
#include "command_queue/queue.h"
#include "command/command.h"

void core0_init()
{

    // initialize stdio and wait for USB CDC connect

    printf("Initializing core 0\n");
    led_init();
}



int connect_to_wifi() {
    if (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK)) {
        blink(ERROR_BLINK_SPEED, ERROR_WIFI);
        printf("Failed to connect to WIFI\n");
        return 1;
    }
    printf("Connected to %s\n", WIFI_SSID);
    blink(MESSAGE_BLINK_SPEED, MESSAGE_CONNECTED);
    return 0;
}

void check_wifi() {
    if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) != CYW43_LINK_JOIN) {
        while (1) {
            if (!connect_to_wifi()) {
                return;
            }
        }
    }
}


void start_wifi()
{
    cyw43_arch_enable_sta_mode();
}

void core0_loop()
{
    sleep_ms(2000);
    printf("Starting core 0 loop\n");
    blink(MESSAGE_BLINK_SPEED, MESSAGE_WELCOME);
    sleep_ms(1000);

    start_wifi(); 
    check_wifi();
    
    command_queue_init();

    sntp_sync_init();
    send_hello();
    print_all_ip_addresses();

    bsp_init();

    char* message;
    uint16_t message_length;
    while(1) {
        sleep_ms(100);
        while (1) {
            if (!command_queue_pop_message(message, &message_length)) {
                puts("No more messages to read");
                break;
            };
            puts("Got a message, going to parse it...");
            if (!parse_command(message, message_length)) {
                puts("Error parsing command :-(");
            }
            free(message);
        }
        led_update();

    }
    cyw43_arch_deinit();
}
