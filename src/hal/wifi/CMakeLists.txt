add_library(beatled_hal_wifi
    wifi.c
)

target_link_libraries(beatled_hal_wifi
    ${PICO_WIFI_LIBRARY}
)

target_include_directories(beatled_hal_wifi PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/..
)
