cmake_minimum_required(VERSION 3.13)

include(cmake/port.cmake)
include(cmake/hal_module.cmake)
include(cmake/utils.cmake)

project(pico_w_beatled C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS c)

port_init()


set(WIFI_SSID "$ENV{WIFI_SSID}" CACHE INTERNAL "WiFi SSID for examples")
set(WIFI_PASSWORD "$ENV{WIFI_PASSWORD}" CACHE INTERNAL "WiFi password for examples")
set(SNTP_SERVER_ADDRESS "raspberrypi1.local")
# set(SERVER_NAME "raspberrypi1.local")
set(SERVER_NAME "Oscars-MacBook-Pro.local")

option(PICO_AUTOTEST "Run with auto tests"   OFF)

IF(PICO_AUTOTEST)
    ADD_DEFINITIONS(-DPICO_AUTOTEST)
ENDIF(PICO_AUTOTEST)


set(BEATLED_PROTOCOL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/beatled_protocol")
include(beatled_protocol_import.cmake)
beatled_protocol_init()

# # Define ARM_CPU, CMSIS ROOT and DSP to use CMSIS-DSP
# set(ARM_CPU "cortex-m0plus")
# set(CMSISCORE ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS_5/CMSIS/Core)
# set(CMSISDSP ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-DSP)

# add_subdirectory(${CMSISDSP}/Source bin_dsp EXCLUDE_FROM_ALL)
# add_subdirectory("lib/microphone-library-for-pico" EXCLUDE_FROM_ALL)

add_subdirectory(src)
add_subdirectory(tests)


