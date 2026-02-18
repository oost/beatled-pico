# Ports

beatled-pico uses a Hardware Abstraction Layer (HAL) with 10 modules, each providing port-specific implementations. This allows the same application code to run on different hardware and OS targets.

## Overview

| Port | Target | Cores | RTOS | Build System |
|------|--------|-------|------|--------------|
| `pico` | Raspberry Pi Pico W (RP2040) | 2 | None (bare-metal) | CMake + Pico SDK |
| `pico_freertos` | Raspberry Pi Pico W (RP2040) | 2 | FreeRTOS SMP | CMake + Pico SDK |
| `posix` | macOS / Linux | 1 | None (pthreads) | CMake |
| `posix_freertos` | macOS / Linux | 1 | FreeRTOS (POSIX sim) | CMake |
| `esp32` | ESP32-S3, ESP32-C3, etc. | 1-2 | FreeRTOS (ESP-IDF) | ESP-IDF (idf.py) |

## Compile Definitions

Each port sets one or more of these defines, which application code can use for conditional compilation:

| Port | `PICO_PORT` | `POSIX_PORT` | `ESP32_PORT` | `FREERTOS_PORT` |
|------|:-----------:|:------------:|:------------:|:---------------:|
| `pico` | x | | | |
| `pico_freertos` | x | | | x |
| `posix` | | x | | |
| `posix_freertos` | | x | | x |
| `esp32` | | | x | x |

## Building

### pico / pico_freertos

Requires the Pico SDK and ARM toolchain:

```bash
cmake -DPORT=posix -B build -S .
cmake --build build

cmake -DPORT=posix_freertos -B build_posix_freertos -S .
cmake --build build_posix_freertos
```

Cross-compile for RP2040 hardware:

```bash
cmake -DPORT=pico -B build_pico -S .
cmake --build build_pico
```

### posix / posix_freertos

Builds a native binary for development and testing on macOS/Linux. The `posix` port includes a Metal-based LED visualization window on macOS.

```bash
cmake -DPORT=posix -B build -S .
cmake --build build

cmake -DPORT=posix_freertos -B build_posix_freertos -S .
cmake --build build_posix_freertos
```

### esp32

Uses a separate ESP-IDF project wrapper in `esp32/`. Requires ESP-IDF v5.x:

```bash
cd esp32
WIFI_SSID=MyNet WIFI_PASSWORD=MyPass BEATLED_SERVER_NAME=192.168.1.100 NUM_PIXELS=30 \
  idf.py build
idf.py flash monitor -p /dev/ttyUSB0
```

The ESP32 port supports both dual-core (S3, original ESP32) and single-core (C3) chips. On single-core chips, the LED task runs on the same core as networking instead of being pinned to core 1.

## HAL Modules

Each module lives under `src/hal/<module>/` with a public header in `include/hal/` and port implementations under `ports/<port_name>/`.

### Module Implementation Matrix

Shows which source file(s) each port uses. Arrows indicate code sharing between ports.

| Module | pico | pico_freertos | posix | posix_freertos | esp32 |
|--------|------|---------------|-------|----------------|-------|
| **blink** | `pico/blink.c` | = pico | `posix/fake_blink.c` | `posix_freertos/blink.c` | `esp32/blink.c` |
| **board** | `pico/unique_id.c` | = pico | `posix/unique_id.c` | = posix | `esp32/unique_id.c` |
| **network** | `pico/udp_*.c, dns.c` | = pico | `posix/udp.c, dns.c, udp_socket.c` | own `udp.c` + posix's `dns.c, udp_socket.c` | `esp32/udp.c` + posix's `dns.c` |
| **process** | `pico/process.c` | `pico_freertos/process.c` | `posix/process.c` | `posix_freertos/process.c` | `esp32/process.c` |
| **queue** | `pico/queue.c` | `pico_freertos/queue.c` | `posix/posix_queue.c` | -> pico_freertos | -> pico_freertos |
| **registry** | `pico/registry.c` | `pico_freertos/registry.c` | `posix/registry.c` | -> pico_freertos | -> pico_freertos |
| **runtime** | `pico/startup.cpp` | `pico_freertos/startup.cpp` | `posix/startup.cpp` + Metal renderer | `posix_freertos/startup.cpp` | `esp32/startup.c` |
| **time** | `pico/time.c, alarm.c` | = pico | `posix/time.c, alarm.c` | `posix/time.c` + `posix_freertos/alarm.c` | `esp32/time.c` + posix_freertos's `alarm.c` |
| **wifi** | `pico/wifi.c` | = pico | `posix/fake_wifi.c` | `posix_freertos/wifi.c` | `esp32/wifi.c` |
| **ws2812** | `pico/ws2812.c` (PIO+DMA) | = pico | `posix/ws2812.cpp` | = posix | `esp32/ws2812.c` (RMT) |

Legend: `= pico` means the same source files as the pico port. `-> pico_freertos` means references `pico_freertos/` source via CMake. `+` means also includes files from another port.

### Code Sharing Patterns

**FreeRTOS queue and registry** are pure FreeRTOS API (no hardware dependencies), so three ports share them:
- `pico_freertos`, `posix_freertos`, and `esp32` all use `pico_freertos/queue.c` and `pico_freertos/registry.c`

**POSIX network core** (BSD sockets, DNS resolution) is shared:
- `posix_freertos` references `posix/udp_socket.c` and `posix/dns.c`
- `esp32` references `posix/dns.c` (ESP-IDF supports `getaddrinfo`)

**FreeRTOS alarm** (software timers) is shared:
- `posix_freertos` and `esp32` both use `posix_freertos/alarm.c`

## Port Details

### pico

Bare-metal port for the RP2040. Uses the Pico SDK directly:
- **LED driving**: PIO state machine generates WS2812 timing, DMA transfers pixel data
- **Networking**: lwIP TCP/IP stack integrated with the CYW43 WiFi driver
- **Multi-core**: Hardware multicore launch via `multicore_launch_core1()`
- **Timing**: Hardware alarm pool from Pico SDK
- **Sync**: Pico SDK mutex (`auto_init_mutex`)
- **Board ID**: Reads unique flash ID from ROM

### pico_freertos

FreeRTOS SMP on RP2040. Replaces bare-metal primitives with FreeRTOS equivalents:
- **Sync**: `xSemaphoreCreateMutex()` instead of Pico mutex
- **Queues**: `xQueueCreate()` instead of Pico `queue_t`
- **Multi-core**: FreeRTOS SMP schedules tasks across both cores
- **WiFi**: Uses `pico_cyw43_arch_lwip_sys_freertos` (thread-safe lwIP)
- Everything else (PIO, DMA, timers, board ID) identical to `pico`

### posix

Development port for macOS/Linux. Replaces hardware with simulation:
- **LED driving**: Renders pixel stream to a Metal window (macOS) showing a simulated LED strip. The runtime module is ~20 files including Metal shaders.
- **Networking**: Standard BSD sockets with pthreads for the UDP listener
- **WiFi**: Stub (`fake_wifi.c`) — assumes network is already available
- **Multi-core**: `pthread_create()` for the LED thread
- **Timing**: POSIX timers (`timer_create`) or `dispatch_source` on macOS
- **Sync**: `pthread_mutex_t`
- **Board ID**: Stub returning a fixed ID

### posix_freertos

Runs the FreeRTOS POSIX/Linux simulator. Used to validate FreeRTOS-specific code paths without hardware:
- **Scheduler**: FreeRTOS GCC POSIX port (simulates preemptive scheduling on pthreads)
- **Queues/Registry**: FreeRTOS primitives (shared from `pico_freertos`)
- **Networking**: Same BSD sockets as `posix`, but UDP listener runs as a FreeRTOS task instead of a pthread
- **Timing**: FreeRTOS software timers for alarms
- **WiFi/Blink**: Stubs

### esp32

ESP-IDF port for ESP32 family chips. ESP-IDF provides FreeRTOS, lwIP, and drivers as components:
- **Build**: Separate `esp32/` directory with ESP-IDF project wrapper. Sources referenced by relative path (`../../src/...`) via `idf_component_register()`
- **LED driving**: ESP-IDF `led_strip` component using the RMT peripheral
- **Networking**: BSD sockets (from lwIP) with FreeRTOS task listener. `esp_netif` for IP queries.
- **WiFi**: Event-driven `esp_wifi` API with `EventGroupHandle_t` for blocking connect
- **Multi-core**: `xTaskCreatePinnedToCore()` on dual-core chips, `xTaskCreate()` on single-core
- **Timing**: `esp_timer_get_time()` for microsecond clock
- **Board ID**: Reads MAC address from eFuse (`esp_efuse_mac_get_default`)
- **Startup**: NVS flash init, no explicit scheduler start needed (ESP-IDF starts FreeRTOS before `app_main`)
- **Fatal errors**: `abort()` triggers ESP-IDF core dump + automatic reboot

## HAL Public API

### blink (`hal/blink.h`)
```c
void blink_once(int speed);
void blink(int speed, int count);
```

### board (`hal/board.h`)
```c
void hal_stdio_init();
void get_unique_board_id(uint8_t *board_id);  // 8-byte output
```

### network (`hal/udp.h`)
```c
typedef int (*process_response_fn)(void *data, uint16_t data_len);
typedef int (*prepare_payload_fn)(void *buf, size_t buf_len);

void start_udp(const char *server_name, uint16_t server_port,
               uint16_t udp_port, process_response_fn process_response);
void shutdown_udp_socket();
const uint32_t get_ip_address();
void udp_print_all_ip_addresses();
int send_udp_request(size_t msg_length, prepare_payload_fn prepare_payload);
```

### process (`hal/process.h`)
```c
typedef void *(*core_loop_fn)();
void start_core1(core_loop_fn core_loop);
void start_isr_thread(core_loop_fn isr_loop);
void join_cores();
void sleep_ms(uint32_t duration);
```

### queue (`hal/queue.h`)
```c
typedef hal_queue_t *hal_queue_handle_t;
hal_queue_handle_t hal_queue_init(size_t msg_size, int queue_size);
void hal_queue_free(hal_queue_handle_t queue);
unsigned int hal_queue_size(hal_queue_handle_t queue);
unsigned int hal_queue_capacity(hal_queue_handle_t queue);
bool hal_queue_add_message(hal_queue_handle_t queue, void *data);
void hal_queue_add_message_blocking(hal_queue_handle_t queue, void *data);
bool hal_queue_pop_message(hal_queue_handle_t queue, void *data);
void hal_queue_pop_message_blocking(hal_queue_handle_t queue, void *data);
```

### registry (`hal/registry.h`)
```c
void registry_init();
void registry_lock_mutex();
void registry_unlock_mutex();
bool registry_try_lock_mutex();
```

### runtime (`hal/startup.h`)
```c
typedef void (*startup_main_t)();
void startup(startup_main_t startup_main);
```

### time (`hal/time.h`)
```c
typedef void (*alarm_callback_fn)(void *user_data);
typedef struct hal_alarm hal_alarm_t;

uint64_t time_us_64(void);
uint64_t get_local_time_us();
hal_alarm_t *hal_add_alarm(int64_t delay_us, alarm_callback_fn callback_fn, void *user_data);
bool hal_cancel_alarm(hal_alarm_t *alarm);
hal_alarm_t *hal_add_repeating_timer(int64_t delay_us, alarm_callback_fn callback_fn, void *user_data);
bool hal_cancel_repeating_timer(hal_alarm_t *alarm);
```

### wifi (`hal/wifi.h`)
```c
void wifi_init();
void wifi_deinit();
void wifi_check(const char *wifi_ssid, const char *wifi_password);
```

### ws2812 (`hal/ws2812.h`)
```c
void ws2812_init(uint16_t num_pixel, uint8_t ws2812_pin, uint32_t frequency, bool is_rgbw);
void output_strings_dma(uint32_t *stream);
```

## Adding a New Port

1. Create directories under each HAL module: `src/hal/<module>/ports/<new_port>/`
2. Implement each module's public API (see headers above)
3. Add a `CMakeLists.txt` in each port directory to register sources
4. Add the port name to `cmake/port.cmake` with appropriate compile definitions
5. For ESP-IDF based ports, create a wrapper project directory instead (see `esp32/` for reference)

Reuse existing implementations where possible — if your port uses FreeRTOS, you can reference `pico_freertos/queue.c` and `pico_freertos/registry.c` directly.

## Directory Structure

```
src/hal/
├── blink/
│   ├── include/hal/blink.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── board/
│   ├── include/hal/{board.h,unique_id.h}
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── network/
│   ├── include/hal/{network.h,udp.h}
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── process/
│   ├── include/hal/process.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── queue/
│   ├── include/hal/queue.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos}/
├── registry/
│   ├── include/hal/registry.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos}/
├── runtime/
│   ├── include/hal/startup.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── time/
│   ├── include/hal/time.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── wifi/
│   ├── include/hal/wifi.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
├── ws2812/
│   ├── include/hal/ws2812.h
│   └── ports/{pico,pico_freertos,posix,posix_freertos,esp32}/
└── utils/
    └── include/hal/utils.h
```

Note: `queue` and `registry` don't have `esp32/` subdirectories — the ESP32 port reuses `pico_freertos/` implementations directly via its CMakeLists.
