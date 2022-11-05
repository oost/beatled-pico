# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include (cmake/hal_module.cmake)


set(WIFI_SSID "$ENV{WIFI_SSID}" CACHE INTERNAL "WiFi SSID for examples")
set(WIFI_PASSWORD "$ENV{WIFI_PASSWORD}" CACHE INTERNAL "WiFi password for examples")
set(SNTP_SERVER_ADDRESS "raspberrypi1.local")
set(SERVER_NAME "Oscars-MacBook-Pro.local")

set(PORT "$ENV{PORT}" )


list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS c)

if(PORT EQUAL "pico")
  set(PICO_PORT ON) 
  add_compile_definitions(PICO_PORT)

else()
  SET(POSIX_PORT ON)
  add_compile_definitions(POSIX_PORT)
  if(STDC_NO_THREADS)
    message( FATAL_ERROR "Need thread support" )
  else()
    message("Threads are supported.")
  endif()
endif()

message("Building ${PORT} port")


if (PICO_PORT)
  # Initialise pico_sdk from installed location
  # (note this can come from environment, CMake cache etc)
  set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/pico-sdk")
  # Pull in Raspberry Pi Pico SDK (must be before project)
  include(pico_sdk_import.cmake)
  pico_sdk_init()
endif()



set(BEATLED_PROTOCOL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/beatled_protocol")
include(beatled_protocol_import.cmake)
beatled_protocol_init()

# # Define ARM_CPU, CMSIS ROOT and DSP to use CMSIS-DSP
# set(ARM_CPU "cortex-m0plus")
# set(CMSISCORE ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS_5/CMSIS/Core)
# set(CMSISDSP ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-DSP)

# add_subdirectory(${CMSISDSP}/Source bin_dsp EXCLUDE_FROM_ALL)
# add_subdirectory("lib/microphone-library-for-pico" EXCLUDE_FROM_ALL)
project(pico_w_beatled C CXX ASM)

add_subdirectory(src)
add_subdirectory(tests)


