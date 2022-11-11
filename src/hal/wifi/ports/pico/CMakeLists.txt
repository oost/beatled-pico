target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    wifi.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
    ${PICO_WIFI_LIBRARY}
)

target_include_directories(${CURRENT_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../..)
