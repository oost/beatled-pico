#include <stdio.h>
#include "pico/stdlib.h"
#include "core0.h"
// #include "hardware/gpio.h"
// #include "hardware/timer.h"
// #include "hardware/clocks.h"

#include "constants.h"
#include "pico/cyw43_arch.h"
#include "ws2812/ws2812.h"

#define BSP_PORT 8765

typedef struct BSP_T_ {
    struct udp_pcb *bsp_pcb;
} BSP_T;


void core0_init()
{

    // initialize stdio and wait for USB CDC connect

    printf("Initializing core 0\n");
    led_init();
}

void blink_once(int speed)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(speed);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(speed);
}

void blink(int speed, int count)
{
    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {
            blink_once(speed);
        }
    }
}

// NTP data received
static void dgram_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    printf("Received UDP datagram\n");

    BSP_T *state = (BSP_T *)arg;
    uint8_t instruction = pbuf_get_at(p, 0);
    char msg[1024];
    u16_t l = pbuf_copy_partial(p, msg, 1024, 0);
    led_set_random_pattern();
    if (l) {
        printf("Received: %d bytes\n", l);
        msg[23] = '\0';
        printf("Received: %s\n", msg);
    } else { 
        puts("Error with msg");
    }

    // // Check the result
    // if (port == BSP_PORT)
    // {
    //     blink(250, 6);
    // }
    // else
    // {
    //     blink(1000, 3);
    // }
    pbuf_free(p);
}

// Perform initialisation
static BSP_T *bsp_init(void)
{
    BSP_T *state = calloc(1, sizeof(BSP_T));
    if (!state)
    {
        printf("failed to allocate state\n");
        return NULL;
    }
    state->bsp_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
    if (!state->bsp_pcb)
    {
        printf("failed to create pcb\n");
        free(state);
        return NULL;
    }

    if (udp_bind(state->bsp_pcb, IP_ANY_TYPE, BSP_PORT))
    {
        printf("failed to bind pcb\n");
        free(state);
        return NULL;
    }
    printf("Setting up UDP callback on %s\n", ipaddr_ntoa(&state->bsp_pcb->local_ip));



    udp_recv(state->bsp_pcb, dgram_recv, state);
    return state;
}

int connect_to_wifi()
{
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000))
    {
        blink(1000, 10);

        printf("failed to connect\n");
        return 1;
    }
    printf("Connected to %s\n", WIFI_SSID);
    blink(250, 4);

    return 0;
}

void core0_loop()
{
    sleep_ms(2000);
    printf("Starting core 0 loop\n");
    blink(100, 3);
    sleep_ms(1000);
    if (connect_to_wifi()) {
        return;
    }
    bsp_init();
    while(1) {
        led_update();
        sleep_ms(250);
    }
    cyw43_arch_deinit();

}
