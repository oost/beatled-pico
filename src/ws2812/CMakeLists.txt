
add_library(ws2812 INTERFACE)

# generate the header file into the source tree as it is included in the RP2040 datasheet

target_sources(ws2812 INTERFACE 
        ws2812.c
        ws2812_patterns.c
)

target_link_libraries(ws2812 INTERFACE
        pico_stdlib 
        beatled_hal_ws2812
)

# pico_add_extra_outputs(pio_ws2812)


# target_include_directories(ws2812 INTERFACE ${CMAKE_CURRENT_LIST_DIR})


