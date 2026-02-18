#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"

#include "hal/process.h"

static TaskHandle_t core1_task_handle;

void start_core1(core_loop_fn core_loop) {
#if SOC_CPU_CORES_NUM > 1
  xTaskCreatePinnedToCore((TaskFunction_t)core_loop, "LED", 4096, NULL,
                          tskIDLE_PRIORITY + 2, &core1_task_handle, 1);
#else
  xTaskCreate((TaskFunction_t)core_loop, "LED", 4096, NULL,
              tskIDLE_PRIORITY + 2, &core1_task_handle);
#endif
}

void start_isr_thread(core_loop_fn isr_loop) {}

void join_cores() {}

void sleep_ms(uint32_t duration) {
  vTaskDelay(pdMS_TO_TICKS(duration));
}
