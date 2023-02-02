target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
  ws2812.cpp
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
beatled_hal_runtime
)