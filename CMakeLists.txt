# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/pico-sdk")
# set(PICO_SDK_PATH "/Users/oscarostlund/Programmation/projects/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)


set(BEATLED_PROTOCOL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/beatled_protocol")
include(beatled_protocol_import.cmake)


# add_subdirectory(lib/beatled_protocol)


project(pico_w_beatled C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
beatled_protocol_init()

# # Define ARM_CPU, CMSIS ROOT and DSP to use CMSIS-DSP
# set(ARM_CPU "cortex-m0plus")
# set(CMSISCORE ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS_5/CMSIS/Core)
# set(CMSISDSP ${CMAKE_CURRENT_LIST_DIR}/lib/CMSIS-DSP)

# add_subdirectory(${CMSISDSP}/Source bin_dsp EXCLUDE_FROM_ALL)
# add_subdirectory("lib/microphone-library-for-pico" EXCLUDE_FROM_ALL)

add_subdirectory(src)
add_subdirectory(tests)


