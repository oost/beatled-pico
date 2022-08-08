#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"

#include "core0.h"
#include "core1.h"
#include "data_queue.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

void core1_entry() {
    core1_init();
    core1_loop();
}


int main( void )
{

    stdio_init_all();
    printf("Hello, multicore_runner using queues!\n");

    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    
    cyw43_arch_enable_sta_mode();


    core0_init();

    multicore_launch_core1(core1_entry);

    core0_loop();

    return 0;
}
