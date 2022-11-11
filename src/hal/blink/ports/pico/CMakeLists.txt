target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    blink.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_stdlib
  ${PICO_WIFI_LIBRARY}
)

# For lwipopts.h
target_include_directories(${CURRENT_LIBRARY_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../../..
)
