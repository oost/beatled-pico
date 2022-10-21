# Add executable. Default name is the project name, version 0.1

if (NOT PICO_CYW43_SUPPORTED AND NOT TARGET pico_cyw43_arch)
        message(FATAL_ERROR "Skipping Pico W examples as support is not available ${PICO_CYW43_SUPPORTED}")
endif()


set(WIFI_SSID "$ENV{WIFI_SSID}" CACHE INTERNAL "WiFi SSID for examples")
set(WIFI_PASSWORD "$ENV{WIFI_PASSWORD}" CACHE INTERNAL "WiFi password for examples")

add_subdirectory(ws2812)

add_executable(pico_w_beatled 
        main.c
        core0.c
        core1.c
        clock/clock.c
        blink/blink.c
        udp_server/udp_server.c
        command_queue/queue.c
        command/command.c
)


# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(pico_w_beatled ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

pico_add_extra_outputs(pico_w_beatled)


pico_set_program_name(pico_w_beatled "pico_w_beatled")
pico_set_program_version(pico_w_beatled "0.1")

pico_enable_stdio_uart(pico_w_beatled 1)
pico_enable_stdio_usb(pico_w_beatled 0)

target_compile_definitions(pico_w_beatled PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )

target_include_directories(pico_w_beatled PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

# Add any user requested libraries
target_link_libraries(pico_w_beatled
        hardware_dma
        hardware_adc
        hardware_pio
        # hardware_interp
        hardware_timer
        hardware_clocks
        pico_multicore
        pico_stdlib
        pico_util
        pico_cyw43_arch_lwip_threadsafe_background
        # pico_cyw43_arch_lwip_poll
        pico_lwip_sntp
        hardware_pio
        ws2812
        )

pico_add_extra_outputs(pico_w_beatled)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-size --format=berkeley "pico_w_beatled.elf")
