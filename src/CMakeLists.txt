# Add executable. Default name is the project name, version 0.1

option(PICO_AUTOTEST "Run with auto tests"   OFF)

IF(PICO_AUTOTEST)
    ADD_DEFINITIONS(-DPICO_AUTOTEST)
ENDIF(PICO_AUTOTEST)

if (NOT PICO_CYW43_SUPPORTED AND NOT TARGET pico_cyw43_arch)
        message(FATAL_ERROR "Skipping Pico W examples as support is not available ${PICO_CYW43_SUPPORTED}")
endif()



add_subdirectory(hal)
add_subdirectory(ws2812)
add_subdirectory(state_manager)
add_subdirectory(command)
add_subdirectory(clock)
add_subdirectory(event_queue)

add_executable(pico_w_beatled 
        main.c
        core0.c
        core1.c
        event_queue/queue.c
        autotest/autotest.c 
) 


# generate the header file into the source tree as it is included in the RP2040 datasheet
# pico_generate_pio_header(pico_w_beatled ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

pico_add_extra_outputs(pico_w_beatled)


pico_set_program_name(pico_w_beatled "pico_w_beatled")
pico_set_program_version(pico_w_beatled "0.1")

pico_enable_stdio_uart(pico_w_beatled 1)
pico_enable_stdio_usb(pico_w_beatled 0)

target_compile_definitions(pico_w_beatled PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        SNTP_SERVER_ADDRESS="${SNTP_SERVER_ADDRESS}"
        SERVER_NAME="${SERVER_NAME}"
        )

target_include_directories(pico_w_beatled PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

# Add any user requested libraries
target_link_libraries(pico_w_beatled
        pico_stdlib
        # pico_lwip_sntp

        ws2812
        state_manager
        commands
        beatled_protocol
        clock
        beatled_hal_blink
        beatled_hal_clock
        beatled_hal_wifi
        beatled_hal_udp
        )

pico_add_extra_outputs(pico_w_beatled)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-size --format=berkeley "pico_w_beatled.elf")
