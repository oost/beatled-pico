
add_library(ws2812 INTERFACE)

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(ws2812 ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

target_sources(ws2812 INTERFACE 
        ws2812.c
        ws2812_patterns.c
        ws2812_dma.c
)

target_link_libraries(ws2812 INTERFACE
        pico_stdlib 
        hardware_pio 
        hardware_dma
)

# pico_add_extra_outputs(pio_ws2812)


# target_include_directories(ws2812 INTERFACE ${CMAKE_CURRENT_LIST_DIR})


