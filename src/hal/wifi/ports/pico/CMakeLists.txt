target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    wifi.c
)

target_link_libraries(beatled_hal_wifi
    ${PICO_WIFI_LIBRARY}
)
