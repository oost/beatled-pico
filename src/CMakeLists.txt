# Add executable. Default name is the project name, version 0.1

message("Building beatled with ${CMAKE_BUILD_TYPE}")

add_subdirectory(autotest)
add_subdirectory(hal)
add_subdirectory(ws2812)
add_subdirectory(state_manager)
add_subdirectory(command)
add_subdirectory(clock)
add_subdirectory(event)
add_subdirectory(process)
add_subdirectory(registry)

add_executable(pico_w_beatled 
        main.c
) 


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
    beatled_ws2812
    beatled_state_manager
    beatled_commands
    beatled_protocol
    beatled_clock
    beatled_hal
    beatled_process
    beatled_autotest
    beatled_registry
)

if (PICO_PORT)
  if (NOT PICO_CYW43_SUPPORTED AND NOT TARGET pico_cyw43_arch)
          message(FATAL_ERROR "Skipping Pico W examples as support is not available ${PICO_CYW43_SUPPORTED}")
  endif()
  # generate the header file into the source tree as it is included in the RP2040 datasheet
  # pico_generate_pio_header(pico_w_beatled ${CMAKE_CURRENT_LIST_DIR}/ws2812/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

  pico_add_extra_outputs(pico_w_beatled)


  pico_set_program_name(pico_w_beatled "pico_w_beatled")
  pico_set_program_version(pico_w_beatled "0.1")

  pico_enable_stdio_uart(pico_w_beatled 1)
  pico_enable_stdio_usb(pico_w_beatled 0)

  print_memsize(${PROJECT_NAME})

endif()

