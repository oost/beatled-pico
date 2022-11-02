add_library(beatled_hal_blink blink.c)

target_link_libraries(beatled_hal_blink
  pico_stdlib
  ${PICO_WIFI_LIBRARY}
)

target_include_directories(beatled_hal_blink PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
